<? 
	session_start(); 
	 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
	 INCLUDE $BASEDIR."main.php";
	 INCLUDE $BASEDIR."sqldb.php";
	 main::sqldb();
	 $db=main::$db;	
	 if(isset($_POST["id"])&&isset($_POST["token"])){
		$id=main::basea_decode($_POST['id'])[1];
		if($r=main::$db->q('select password from users where id='.$id))
		{
			$password=$r->row()['password'];
			if(main::passhash($password)==$_POST['token'])
				$_SESSION['id']=$id;
			main::$db->q("update users set online=1,longitude=null,latitude=null where id=".$id);
		}  
	 }
	if(!isset($_SESSION['id']))
		header("Location:/"); 
	if(isset($_POST['sendMessage'])&&isset($_POST['recipient'])){
		$db->q("insert into messages(message,sender,recipient) values('".$db->safe($_POST['sendMessage'])."',".$_SESSION['id'].",".main::basea_decode($_POST['recipient'])[1].")");
		exit;
	}
	else if(isset($_POST['getMessage'])){
		$recipient=main::basea_decode($_POST['getMessage'])[1];
		$name="";
		if($name=$db->q("select user_name from users where id=".$recipient))
			$name=$name->row()['user_name'];
		$html="";
		$lastView=$_POST['lastView'];
		if($r=$db->q("select id,message,sender from messages where id >".$_POST['lastView']." and ((sender=".$recipient." and recipient=".$_SESSION['id'].") OR (recipient=".$recipient." and sender=".$_SESSION['id'].")) order by recieve_date asc")){
			while($row=$r->row()){
				$html.= "<div><p class='".($row['sender']==$_SESSION['id'] ? 'outgoing': 'incoming')."'>".($row['sender']!=$_SESSION['id'] ? $name.': ': ' You: ').$row['message']."</p></div>";
				$lastView=$row['id'];
			}
		}
		$message=array();
		$message["html"]=$html; 
		$message["lastid"]=$lastView; 
		echo json_encode($message);
		exit;
	} 
	if(isset($_POST['getStatus'])){
		$status=array();
		$status["status"]=array();
		$status["total"]=0;
		$query="select u.id id,
				   u.user_name name,
				   u.online online,
				   u.latitude lat,
				   u.longitude lon
			FROM friends fr 
			INNER JOIN users u on u.id=fr.friendsuserid
			where fr.userid=".$_SESSION['id']; 
		if($r=$db->q($query)){
			while($row=$r->row()){
				$temp=array();
				$temp["id"]=main::basea_encode(array(1,$row['id'],1));
				$temp["status"]=($row["online"]==0 ? "offline" : "online");
				$temp["name"]=$row["name"];
				$status["total"]++;
				array_push($status["status"],$temp);
			}
		}
		$db->q("update users u join friends fr on u.id=fr.friendsuserid  set online=1 where online=2 and fr.userid=".$_SESSION['id']);
		echo json_encode($status);
		exit;
	} 
	if(isset($_POST['getLocations'])){
		$status=array();
		$status["status"]=array();
		$status["total"]=0;
		$query="select distinct u.id id,  
				   u.latitude lat,
				   u.longitude lon,
				   u.online online
			FROM friends fr 
			INNER JOIN users u on u.id=fr.friendsuserid
			where fr.userid=".$_SESSION['id']; 
		if($r=$db->q($query)){
			while($row=$r->row()){
				$temp=array(); //u.online>0 and latitude is not null and longitude is not null and 
				$temp["id"]=main::basea_encode(array(1,$row['id'],1)); 
				if($row['online']>0){ 
					$temp["lat"]=$row["lat"];
					$temp["lon"]=$row["lon"];
				}else{ 
					$temp["lat"]="Not Available";
					$temp["lon"]="Not Available";
				}
				$status["total"]++;
				array_push($status["status"],$temp);
			}
		}
		echo json_encode($status); 
		exit;
	}
?>
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false&libraries=geometry"></script>
<style>
	body{
		width:100%;
		height:100%;
		padding:0;
		margin:0;
		background:#2c3e50;
		overflow-x:scroll;
		overflow-y:hidden;
	}
	#userStatus{
		width:50%;
		float:left; 
		padding:0;
		margin:0;
		height:100%;
		overflow:hidden;
		overflow-y:scroll;
	}
	#chat{
		width:50%;
		float:right;
		height:100%;
		padding:0;
		margin:0;
		overflow:hidden;
	}
	#justsignin{
		position:fixed;
		width:100%;
		top:0;
		left:0;
		background:#FFF;
		color:#000;
		font-size:20px;
		padding-top:10px;
		padding-bottom:10px;
		padding-right:5px;
		padding-left:5px;
		z-index:10;
		display:none;
	}
</style>
<html>
	<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<script src="http://code.jquery.com/jquery-latest.min.js" type="text/javascript"></script>   
	<body>
		<div id="justsignin"></div>
		<div id="bodyHolder" style="width:200%;height:100%;overflow:hidden;">
			<div id="userStatus" style="background:#18bc9c">
				 <? include("status.php");?>
			</div>
			<div id="chat">
				<? include("chat.php");?>
			</div>
		</div>
		<? include("scripts.php");?>
	</body>
</html>
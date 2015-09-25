<?
	//print_r($_POST);exit;
 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
 INCLUDE $BASEDIR."main.php";
 INCLUDE $BASEDIR."sqldb.php";
 main::sqldb();
 $db=main::$db;
 if(isset($_POST['check_phone'])){
	 $phone=main::deformat_phone($_POST['check_phone']);
	 $status=array(); 
	 if(is_numeric($phone)&&strlen($phone)==10)
	 {
		if(main::$db->q("select 1 from users where phone='".main::$db->safe($phone)."'")){
			$status['status']=0; 
			$status['error']="Phone Number already in use.";
		}else{
			$status['status']=1;
		}
	 }
	 else{
		 $status['status']=0;
		 $status['error']="Invalid Phone";
	 }
	 echo json_encode($status);
	 exit;
 }
 if(isset($_POST['setLon'])&&isset($_POST['setLat'])){
	 $id=main::basea_decode($_POST['id']);
	 if($r=main::$db->q('select password from users where id='.$id[1])){
		 $password=$r->row()['password']; 
		 if(main::passhash($password)==$_POST['token']){
			 echo main::$db->q("update users set latitude='".$db->safe($_POST['setLat'])."' , longitude='".$db->safe($_POST['setLon'])."' where id=".$id[1]); 
		 }
	 } 
	 exit;
 }
 if(isset($_POST['register'])&&isset($_POST['password'])){
	 $status=array();
	 $status["status"]=0;
	 if(strlen(trim($_POST['username']))<3){
		 if(strlen(trim($_POST['password']))<3){
			if($_POST['password']==$_POST['confirmpassword']){
				if(!$db->q("select 1 from users where user_name='".$db->safe($_POST['username'])."'")){
					$db->q("insert into users (online,user_name,password,phone,active,name)values(1,'".$db->safe($_POST['username'])."','".main::passhash($_POST['password'])."','".$db->safe($_POST['phone'])."',1,'".$db->safe($_POST['firstname']." ".$_POST['lastname'])."')");
					$status['status']=1;
					$status['id']=main::basea_encode(array(rand(),main::$db->id(),date('ndyj')));
					$status['token']=main::passhash(main::passhash($_POST['password']));
				}else $status["status"]="User name already exist";
			}else $status["status"]="Password & Confirm password does not match";
		}else $status["status"]="Password must be at least three characters long";
	 }else $status["status"]="User name must be at least three characters long";
	echo json_encode($status);
	 exit;
 }
 if(isset($_POST['getContactList'])){
	 $id=main::basea_decode($_POST['id']);
	 if($r=main::$db->q('select password from users where id='.$id[1])){
		 $password=$r->row()['password'];
		 if(main::passhash($password)==$_POST['token']){
			 $query="select u.id id,
		               u.name name,
					   u.online active
				FROM friends fr 
				INNER JOIN users u on u.id=fr.friendsuserid
				where fr.userid=".$id[1];
			if($r=main::$db->q($query))
			{
				$status=array();
				$cnt=0;
				while($row=$r->row()){
					$status["u".$cnt]=array(main::basea_encode(array(rand(),$row['id'],rand())),$row['name'],($row['active']==1 ? ' online' :' offline'));
					$cnt++;
				} 
				$status['total']=$cnt;
				echo json_encode($status);
			} 
		 }
	 }
	 exit;
 }  
 if(isset($_POST['getMessages'])&&isset($_POST['token'])&&isset($_POST['id'])&&isset($_POST['to'])){
	$id=main::basea_decode($_POST['id'])[1];
	$to=main::basea_decode($_POST['to'])[1];
	$total=0;
	$status=array();
	if($r=main::$db->q('select password from users where id='.$id))
	{
		$password=$r->row()['password'];
		if(main::passhash($password)==$_POST['token']){//sender
			$db=main::$db;
			if($r=$db->q("select t1.* from (select * from messages where (sender='".$id."' AND recipient='".$to."') OR (sender='".$to."' AND recipient='".$id."') order by recieve_date desc limit 30)t1 order by t1.recieve_date asc"))
			{
				while($row=$r->row()){
					$status["u".$total]=array(($row['recipient']==$id ? 'incoming' : 'outgoing'),$row['message']);
					$total++;
				}
			} 
		}
	}  
	$status["total"]=$total;
    echo json_encode($status);
	exit;
 }
 if(isset($_POST['sendMessage'])&&isset($_POST['message'])&&isset($_POST['token'])&&isset($_POST['id'])&&isset($_POST['to'])){
	$id=main::basea_decode($_POST['id']);
	 if($r=main::$db->q('select password from users where id='.$id[1])){
		 $password=$r->row()['password'];
		if(main::passhash($password)==$_POST['token']){
			$db=main::$db;
			$query="insert into messages(message,sender,recipient) values('".$db->safe($_POST['message'])."','".$id[1]."',".main::basea_decode($_POST['to'])[1].")";
			main::$db->q($query);
		 }
	 }  
	exit;
 }
 if(isset($_POST['username'])&&isset($_POST['password'])){
	$status=array(); 
	if(strlen(trim($_POST['username']))>0&&strlen(trim($_POST['password']))>0){
		if($r=$db->q("select id,name,password from users where user_name='".$db->safe($_POST['username'])."' and password='".main::passhash($_POST['password'])."'")){
			$row=$r->row(); 
			$status['status']=1;
			$status['id']=main::basea_encode(array(rand(),$row['id'],date('ndyj')));
			$status['token']=main::passhash($row['password']);
		 }else  $status['status']=0;
		 
    }else  $status['status']=0;
     echo json_encode($status);
    exit;
 } 
?>
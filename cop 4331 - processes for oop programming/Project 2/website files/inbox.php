<?  
session_start();
	
	 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
	 INCLUDE $BASEDIR."main.php";
	 INCLUDE $BASEDIR."sqldb.php";
	 main::sqldb();
	 $db=main::$db;	
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
				if($row['online']>0 && strlen(trim($row["lon"]))>0&&strlen(trim($row["lat"]))>0){ 
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
	if(isset($_POST['getActiveFriends'])){
		$status=array();
		$status["status"]=array();
		$status["total"]=0;
		$query="select distinct u.id id,
				   u.user_name name,
				   u.online online 
			FROM friends fr 
			INNER JOIN users u on u.id=fr.friendsuserid
			where fr.userid=".$_SESSION['id']; 
		if($r=$db->q($query)){
			while($row=$r->row()){
				if($row['id']!=$_SESSION['id']){
					$temp=array(); 
					$temp["id"]=main::basea_encode(array(1,$row['id'],1));
					$temp["status"]=($row["online"]==0 ? "offline" : "online");
					$temp["name"]=$row["name"];
					$status["total"]++;
					array_push($status["status"],$temp);
					
				}
			}
		}
		$db->q("update users u join friends fr on u.id=fr.friendsuserid  set online=1 where online=2 and fr.userid=".$_SESSION['id']);
		echo json_encode($status); 
		exit;


	}else if(isset($_POST['getMessage'])){
		if(isset($_SESSION['id'])){ 
			if($r=main::$db->q("select user_name from users where id=".$_SESSION['id']))
		       $title=$r->row()['user_name'];
			$time=time();
			$getID=main::basea_decode($_POST['getMessage'])[1];
			if($db->q("update messages set view_date=FROM_UNIXTIME(".$time.")  where view_date='0000-00-00 00:00:00' and recipient=".$_SESSION['id']." and sender=".$getID)){
				if($r=$db->q("select message from messages where view_date=FROM_UNIXTIME(".$time.") and recipient=".$_SESSION['id']." and sender=".$getID)){
					while($row=$r->row())
						echo '<div class="message left"><audio autoplay> <source src="blop.mp3" type="audio/mpeg">
                        </audio>'.$row['message'].'</div>';
				}
			} 
		}

         else{
			echo "0";
		}
		exit;
	}

	else if(isset($_POST['setLong'])&&isset($_POST['setLat'])){
		main::$db->q("update users set latitude='".$db->safe($_POST['setLat'])."' , longitude='".$db->safe($_POST['setLong'])."' where id=".$_SESSION['id']);
		exit;
	}
	else if(isset($_POST['sendMessage'])&&isset($_POST['recipient'])){
		$db->q("insert into messages(message,sender,recipient) values('".$db->safe($_POST['sendMessage'])."',".$_SESSION['id'].",".main::basea_decode($_POST['recipient'])[1].")");
		echo "insert into messages(message,sender,recipient) values('".$db->safe($_POST['sendMessage'])."',".$_SESSION['id'].",".main::basea_decode($_POST['recipient'])[1].")";
		exit;
	}else if(isset($_POST['getAllMessage'])){
		$recipient=main::basea_decode($_POST['getAllMessage'])[1];
		if($r=$db->q("select message,sender from messages where (sender=".$recipient." and recipient=".$_SESSION['id'].") OR (recipient=".$recipient." and sender=".$_SESSION['id'].") order by recieve_date asc")){
			while($row=$r->row())
				echo "<div class='message ".($row['sender']==$_SESSION['id'] ? 'right': 'left')."'>".$row['message']."</div>";
		}else echo "<div id='nonew'>You have no messages to display</div>";
		exit;
	}else if(isset($_POST['addFriend'])){
		$id=main::basea_decode($_POST['addFriend'])[1];
		if(is_numeric($id)){
			$db->q('insert into friends values('.$id.','.$_SESSION['id'].')');
			$db->q('insert into friends values('.$_SESSION['id'].','.$id.')');
		}
		exit;
	}
	if(!isset($_SESSION['id']))
		header("Location:/");
	$title='Friendster';
	if($r=main::$db->q("select user_name from users where id=".$_SESSION['id']))
	   $title=$r->row()['user_name'];

?>


<!DOCTYPE HTML>
<html>
    <head>
        <title>Inbox</title>
        <meta charset='utf-8'>
        <meta name="viewport" content="width=320"/>
        <script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false&libraries=geometry"></script>
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" type="text/css" href="inbox.css">
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
        <script src="http://code.jquery.com/jquery-latest.min.js" type="text/javascript"></script>    
	 <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
    </head>
    
    <body>
        
        <!-- Navigation -->
    <nav class="navbar navbar-default navbar-inverse navbar-fixed-top">
        <div class="container">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header page-scroll">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#page-top"><?=$title;?></a>
            </div>

            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                  <ul class="nav navbar-nav navbar-right">
                      
                <li><a onclick="$('#settingsConfirmDiv').show();$('#friendsDiv').hide()"><span class="glyphicon glyphicon-user">
                    </span> Settings</a></li>                            
                      
                 <li><a onclick="$('#locationDiv').show();"><span class="glyphicon glyphicon-globe">
                    </span> Location</a></li>      
                      
                <li><a onclick="$('#friendsDiv').show();
                                $('#settingsDiv').hide()"><span class="glyphicon glyphicon-plus">
                    </span> Add Friend</a></li>
                      
                      
                <li><a onclick="$('#msgBox').empty()"><span class="glyphicon glyphicon-envelope">
                    </span> Clear Messages</a></li>
                      
                      
                <li><a onclick="window.location.href='logout.php'"><span class="glyphicon glyphicon-log-in">
                    </span> Logout</a></li>
                      
                      
              </ul>
            </div>
            <!-- /.navbar-collapse -->
        </div>
        <!-- /.container-fluid -->
    </nav>

        
     
       
        <div class="main">    
        
          
            
            <div class="people">
                <div id="friendslist">
				<?
					$query="select distinct u.id id,
								   u.user_name name
							FROM friends fr 
							INNER JOIN users u on u.id=fr.friendsuserid
							where fr.userid=".$_SESSION['id'];
								
					if($r=$db->q($query))
						while($row=$r->row())
						{
							
							if($row['id']!=$_SESSION['id']){
					?>
						<div id="<?=main::basea_encode(array(1,$row['id'],1));?>"> <?=$row['name'];?><span style="font-size:12px"></span></div>	
					<?		} 
						} 
				?>
				</div>
            </div>
            
            <div id="msgBox" class="messages">Messages</div>            
            <div class="new">
                <div class="input-wrapper">
                    <input type="text" id="newmessage" placeholder="Send a Message">
                </div> 
                <button class="btn" onclick="sendMessage()">Send</button>
            </div>
        </div>
		
		<div id="friendsDiv">
			<p id="addBanner"><font size="4">Friendster ID : <?=main::basea_encode(array(1,$_SESSION['id'],1));?></p>
			<input id="friendsInput" placeholder="Enter Friends ID here to add them"/>
			<button class="btn" onclick="addFriend()">Add Friend</button>
			<button class="btn" onclick="$('#friendsDiv').hide()">Close</button> 
		</div>

        <div id="locationDiv">
            <article id="map"> <span id="status">Friendster is finding your location...</span> </article>
			<iframe id="mapLocation"></iframe>
            <button id="mapClose" class="btn" onclick="$('#locationDiv').hide()">Close</button> 
        </div>
        
		<div class="settings" id="settingsConfirmDiv">
			<p>Confirm your Identity (Required)</p>
			<input id="confirmIdentityPass" placeholder="Enter Your Password">
			<button class="btn" onclick="confirmIdentity()">Submit Changes</button>
			<button class="btn" onclick="$('#settingsConfirmDiv').hide()">Close</button> 
		</div>

		<div class="settings" id="settingsDiv">
			<h2>Settings</h2>
			<p>Change Username (Optional)</p>
			<input id="newName" placeholder="Enter New Username Here">
			<input id="confirmNewName" placeholder="Confirm New Username">
			<button class="btn" onclick="$('#settingsDiv').hide()">Update Username</button> 
			<p>Change Password (Optional)</p>
			<input id="newPassword" placeholder="Enter New Password Here">
			<input id="confirmNewPassword" placeholder="Confirm New Password">
			<button class="btn" onclick="$('#settingsDiv').hide()">Update Password</button>
			<button class="btn" onclick="$('#settingsDiv').hide()">Close</button> 
		</div>
		
<script>
    getActiveFriends(); 
	$("#friendslist div").click(function(){
		setFriendId($(this).attr("id"));
	});
	function addFriend(){
		$.post("/inbox.php",{
			addFriend:$("#friendsInput").val()
		},function(data){
			$('#friendsDiv').hide();
		});
	}
	function getActiveFriends(){ 
		$.ajax({
		  type: "POST",
		  url: "/inbox.php",
		  data: {
					getActiveFriends:"m"
				},
		  success: function(data){  
        			  var justsignin=""; 
					for(var i=0;i<data.total;i++){ 
						$("#"+data.status[i].id).attr('class',data.status[i].status); 
						/*if ($("#"+data.status[i].id+" img").attr('src')!=(data.status[i].status+'.png'))
							justsignin+=data.status[i].name+"<br/>";*/
					}
					getActiveFriends();
				},
		  dataType: "json"
		});
	}
	function getMessages(){
		$.post("/inbox.php",{
			getMessage:friendId
		},function(data){ 
			if($.trim(data)!="")
			{ 
				if($("#nonew").length>0)
					$(".messages").html(data);
				else
					$(".messages").append(data);
			}
			getMessages();
		});
	}
	var friendId="0";
	function setFriendId(id){
		friendId=id;
		$.post("/inbox.php",{
			getAllMessage:id
		},function(data){
			$(".messages").html(data);
			getMessages();
			$("#newmessage").attr('disabled',false);
		});
	}
	function sendMessage(){
		if ($.trim($('#newmessage').val())){
			console.log("as");
			$.post("/inbox.php",{
				sendMessage:$("#newmessage").val(),
				recipient:friendId
			},function(data){ 
				if($("#nonew").length>0)
					$(".messages").html('<div class="message right">'+$("#newmessage").val()+'</div>');
				else
					$(".messages").append('<div class="message right">' + '<label> You : </label>' + ' ' + $("#newmessage").val()+'</div>' + '<audio autoplay> <source src="plop.mp3" type="audio/mpeg">' + '</audio>');
				$("#newmessage").val("");
			});
		};
    }
    function confirmIdentity(){
		// Post to server checking username and password. Show settings Panel if it returns successful.
		$('#settingsConfirmDiv').slideUp(500);
		$('#settingsDiv').slideDown(750);
	}
	function success(position) { 
		currentPosition= new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
		$.post("/inbox.php",{
			setLong:position.coords.longitude,
			setLat:position.coords.latitude,
		},function(data){
			getLocations();			
		}); 
	  
	} 
	if (navigator.geolocation) {
	  navigator.geolocation.getCurrentPosition(success);
	} else {
	  error('not supported');
	} 
	var currentPosition;  
    
	function getLocations(){
		$.ajax({
		  type: "POST",
		  url: "/inbox.php",
		  data: {
					getLocations:"m"
				},
		  success: function(data){  
		  console.log(data.total);
					for(var i=0;i<data.total;i++){ 
						if(isNaN(data.status[i].lat)|| isNaN(data.status[i].lon)){
							$("#"+data.status[i].id+" span").html("<br>Location unavailable.");
						}
						else
						$("#"+data.status[i].id+" span").html("<br>Distance from you: "+
										(google.maps.geometry.spherical.computeDistanceBetween(
										currentPosition, new google.maps.LatLng(data.status[i].lat, data.status[i].lon)) ).toFixed(2)+" meters"); 
					} 
					setTimeout(function(){ getLocations(); }, 60000);
				},
		  dataType: "json"
		});
	}
	</script>
		
   </body>
</html>

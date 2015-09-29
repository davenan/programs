<?
	if(isset($_POST['username']) && isset($_POST['password'])&& isset($_POST['type'])){
		session_start(); 
		include 'login_session.php';  
		require_once("connect2.php");
		if($_POST['type']==0)
		{
			$response = $dbc->query("select * from user_accts WHERE username='".$_POST['username']."' and password='".$_POST['password']."'" );  
			if($response->num_rows < 1)  
			{ 
			   echo "0"; 
			}

			else{ 
				$userData = $response->fetch_assoc();
				validateUser($userData);
				echo "1"; 
			} 
		}
		else
		{
			$response = $dbc->query("select * from user_accts WHERE username='".$_POST['username']."'" );  
			if($response->num_rows < 1)  
			{ 
			  $dbc->query("INSERT INTO user_accts (username, password) VALUES ('".$_POST['username']."', '".$_POST['password']."')");
			  $userData['username']=$_POST['username'];
			  $userData['password']=$_POST['password'];
			  validateUser($userData);
			  echo "1";
			}

			else{  
				echo "0"; 
			} 
		}
		$dbc->close();
		exit;
	}
?>
<html id="main">

<head>
<style>
#main {
	background-image: url("./blue_texture.jpeg");
	color:white;
	opacity:100px;
	padding:150px;
}
</style>
<script src="//code.jquery.com/jquery-1.11.3.min.js"></script>
</head>

<body>
    <script>
		function login(type){
			if($.trim($("#password"+type).val())==""||$.trim($("#username"+type).val())=="")
			{
				$("#result"+type).css("color","red");
				$("#username"+type+",#password"+type).css("border-color",'red');
				$("#result"+type).html("User name and password are required");
				$("#result"+type).fadeIn(1000); 
			}
			else
			{
				$("#result"+type).css("display","none");
				$("#username"+type+",#password"+type).css("border-color",'black'); 
				$("#result"+type).css("color","green");
				if(type==0)
					$("#result"+type).html("Login in please wait...");
				else
					$("#result"+type).html("Creating account please wait...");
				$("#result"+type).fadeIn(1000);
				$.post("/index.php", {type:type,username:$("#username"+type).val(),password:$("#password"+type).val()
				}, function(result){
					if(result==1){
						$("#username"+type+",#password"+type).css("border-color",'green');
						if(type==1)
							$("#result"+type).html("Account created.Loging you in please wait...");
						setTimeout(function(){ window.location.href="welcome.php"; }, 1000);
						
					}
					else{
						$("#result"+type).css("color","red");
						$("#username"+type+",#password"+type).css("border-color",'red');
						if(type==0)
							$("#result"+type).html("Your login information is incorrect. Please try again");
						else
							$("#result"+type).html("This User Name exist already, Please try again");
						$("#result"+type).fadeIn(1000); 
					}
				});
			}
		}
	</script>
	<div id="loginbox">
		 <h2 style="font-family:Arial;color:#000"> Login </h2>
		<input type="text" id="username0" placeholder="Enter your User Name"style="width:80%;padding:10px;border:1px solid #000;background:#FFF">
		<input type="password" id="password0" placeholder="Enter your Password"style="width:80%;padding:10px;border:1px solid #000;background:#FFF;margin-top:10px">
		<p id="result0"></p>
		<div style="width:100%;text-align:center;margin-top:10px">
			<button onclick="login(0)">Login</button>
			<button onclick="$('#loginbox').css('display','none');$('#registration').fadeIn(1500);">Create New Account</button>
		</div>
	</div> 
	<div id="registration">
		 <h2 style="font-family:Arial;color:#000"> Create Your Account Today </h2>
		<input type="text" id="username1" placeholder="Enter your User Name"style="width:80%;padding:10px;border:1px solid #000;background:#FFF">
		<input type="password" id="password1" placeholder="Enter your Password"style="width:80%;padding:10px;border:1px solid #000;background:#FFF;margin-top:10px">
		<p id="result1"></p>
		<div style="width:100%;text-align:center;margin-top:10px">
			<button onclick="login(1)">Create Account</button> 
			<button onclick="$('#registration').css('display','none');$('#loginbox').fadeIn(1500);">Back</button>
		</div>
	</div> 
</body>
<style>
#loginbox button,#registration button{
	font-size:16px;
	padding:5px;
	cursor:pointer
}
#loginbox,#registration{
	width:600px;
	background:#FFF;
	margin:auto;
	border-radius:5px;
	border:1px solid #000;
	text-align:center;
	padding:10px;
}
#registration{
	display:none;
}
</style>
</html>

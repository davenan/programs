<?  
session_start();
 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
 INCLUDE $BASEDIR."main.php";
 INCLUDE $BASEDIR."sqldb.php";
 main::sqldb();
 $db=main::$db;
 if(isset($_POST['login'])&&isset($_POST['username'])&&isset($_POST['password'])){
	 if(true){
		if($r=$db->q("select id,name from users where user_name='".$db->safe($_POST['username'])."' and password='".main::passhash($_POST['password'])."'")){
			 $row=$r->row();
			 $_SESSION['id']=$row['id'];
			 $_SESSION['name']=$row['name']; 
			 echo 1;
			 $db->q("update users set online=1 where user_name='".$db->safe($_POST['username'])."' and password='".main::passhash($_POST['password'])."'");
			exit;
		 } 
	 }
	 echo 0;
	 exit;
 } 
 else if(isset($_POST['register'])){
	 if(!$db->q("select 1 from users where user_name='".$db->safe($_POST['username'])."'")){
		 $db->q("insert into users (online,user_name,password,phone,active,name)values(1,'".$db->safe($_POST['username'])."','".main::passhash($_POST['password'])."','".$db->safe($_POST['phone'])."',1,'".$db->safe($_POST['name'])."')");
		$_SESSION['id']=$db->id();
		 $_SESSION['name']=$_POST['name']; 
		 echo 1;
	 }
	 else 0;
	 exit;
 }
 //$db->q("update users set password='".main::passhash("t")."'");
?>
<!DOCTYPE html>
<html lang="en">

<head>

    <title>Welcome to Friendster!</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">


    <!-- Bootstrap Core CSS - Uses Bootswatch Flatly Theme: http://bootswatch.com/flatly/ -->
    <link href="homepage/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="homepage/css/freelancer.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="homepage/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Montserrat:400,700" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Lato:400,700,400italic,700italic" rel="stylesheet" type="text/css">
    
    
    <!-- jQuery -->
    <script src="homepage/js/jquery.js"></script>

    <!-- Bootstrap Core JavaScript -->
    <script src="homepage/js/bootstrap.min.js"></script>

    <!-- Plugin JavaScript -->
    <script src="http://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.3/jquery.easing.min.js"></script>
    <script src="homepage/js/classie.js"></script>
    <script src="homepage/js/cbpAnimatedHeader.js"></script>

    <!-- Contact Form JavaScript -->
    <script src="homepage/js/jqBootstrapValidation.js"></script>
    <script src="homepage/js/contact_me.js"></script>

    <!-- Custom Theme JavaScript -->
    <script src="homepage/js/freelancer.js"></script>

</head>

    
    
<body id="page-top" class="index">

     <!-- Navigation -->
    <nav class="navbar navbar-default navbar-fixed-top">
        <div class="container">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header page-scroll">
                <button type="button" class="navbar-toggle" data-toggle="collapse" 
                        data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#page-top">Friendster</a>
            </div>

            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav navbar-right">
                    <li class="hidden">
                        <a href="#page-top"></a>
                    </li>
                    <li class="page-scroll">
                        <a href="#login">Login</a>
                    </li>
                </ul>
            </div>
            <!-- /.navbar-collapse -->
        </div>
        <!-- /.container-fluid -->
    </nav>

    
    <!-- Header [Chat with Friends etc.] -->
    <header>
        <div class="container">
            <div class="row">
                <div class="col-lg-12">
                    <img class="img-responsive" src="homepage/img/chat-2-icon.png" alt="">
                    <div class="intro-text">
                        <span class="name">Chat With Friends</span>
                        <hr class="star-light">
                    </div>
                </div>
            </div>
        </div>
    </header>

    <!-- Login Section -->
    <section id="login_section">
        
        <div class="login" id="login">
            
            <div class="container">
            <div class="row">
                <div class="col-lg-12 text-center">
                    <h2>Login</h2>
                    <hr class="star-primary">
                </div>
            </div>
                
                <div class="col-lg-8 col-lg-offset-2">
                	<div id="error"></div>
                    <form name="sentMessage" id="contactForm" novalidate>
                      
                        <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Login Username</label>
                                <input type="text" class="form-control" placeholder="Username" id="username" 
                                required data-validation-required-message="Please enter your username.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                        <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Login Password</label>
                                <input type="password" class="form-control" placeholder="Password" id="password" 
                                required data-validation-required-message="Please enter your password.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                        <br>
                        <div id="success"></div>
                        <div class="row">
                            <div class="form-group col-xs-12">
                                <button type="submit" class="btn btn-success btn-lg" onclick="validate()">Login</button>

                                <button type="submit" class="btn btn-success btn-lg"
                                onclick="$('#login').css('display','none');$('#create').fadeIn(750);" >Create 
                                Account</button>

                            </div>
                        </div>
                    </form>
                </div>
            </div>            
        </div>
        
        
        
        
         <div class="create" id="create" style="display:none">
             
            <div class="container">
            <div class="row">
                <div class="col-lg-12 text-center">
                    <h2>Create Account</h2>
                    <hr class="star-primary">
                </div>
            </div>
                
                <div class="col-lg-8 col-lg-offset-2">
                    <div id="error2"></div>
                    <form name="sentMessage">
                      
                         <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>First Name</label>
                                <input type="text" class="form-control" placeholder="First Name " id="fname" 
                                required data-validation-required-message="Please Enter a First Name.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                         <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Last Name</label>
                                <input type="text" class="form-control" placeholder="Last Name" id="lname" 
                                required data-validation-required-message="Please Enter a Last Name.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                         <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Phone Number</label>
                                <input type="tel" class="form-control" placeholder="Phone Number" id="phone" 
                                required data-validation-required-message="Please Enter a Phone Number.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                        <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Username</label>
                                <input type="text" class="form-control" placeholder="Username" id="username1" 
                                required data-validation-required-message="Please Enter a Username">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                        <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Password</label>
                                <input type="password" class="form-control" placeholder="Password" id="password1" 
                                required data-validation-required-message="Please enter a Password.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                         <div class="row control-group">
                            <div class="form-group col-xs-12 floating-label-form-group controls">
                                <label>Confirm Password</label>
                                <input type="password" class="form-control" placeholder="Confirm Password" id="cpassword1" 
                                required data-validation-required-message="Please Confirm your Password.">
                                <p class="help-block text-danger"></p>
                            </div>
                        </div>
                        
                        
                        <br>
                        <div id="success"></div>
                        <div class="row">
                            <div class="form-group col-xs-12">
                                <button type="submit" class="btn btn-success btn-lg" onclick="login(1)">
                                Create Account</button>
                                <button onclick="$('#create').css('display','none');$('#login').fadeIn(750);"
                                class="btn btn-success btn-lg">Back</button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>            
        </div>
    </section>
    
    
  
    <script>
    		
    		function validate(){
    			var u = $.trim($("#username").val());
    			var p = $.trim($("#password").val());  
    			
    			if(u.length > 0 && p.length > 0){
    				login(0);
    			}	
    		}
    
            function login(type){
                if(type == 0){
                
                if($("#username").val() == null && $("#password").val() == null){
						//$("#password1,#cpassword1").css("border","1px solid #F000");
						$("#error").val("Password does not match");
					}
					
				else{
                        $("#error").text("Logging you in...");
						$.post("/",{
							username:$("#username").val(),
							password:$("#password").val(),
							login:1
						},function(data){
							console.log(data);
							var result=parseInt(data);
							if(result==1){ 
								window.location.href="inbox.php"; 
							}
							else{
								$("#login input").css("border","1px solid #F00");
								$("#error").text("Incorrect Username or password");
							}
						});
						
					}
                } 
                else{ 
					$("#error2").val("This is creating an account...");
					if($("#password1").val()!=$("#cpassword1").val()){
						$("#password1,#cpassword1").css("border","1px solid #F000");
						$("#error2").val("Password does not match");
					}
					else
					{
							console.log("asdfg");
						$.post("/",{
							username:$("#username1").val(),
							password:$("#password1").val(),
							register:'1',
							name:$("#fname").val()+" "+$("#lname").val(),
							phone:$("#phone").val()
						},function(data){
							console.log(data);
							var result=parseInt(data);
							if(result==1){ 
								window.location.href="/inbox.php"; 
							}
							else{
								$("#error2").val("Incorrect Username or password");
							}
						});
					}
                }
            }
        </script>
    </body>
</html>

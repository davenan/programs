<html id='main'>
    <head>
        <title>Create a new Survey</title>
<style>
        #main {
            background-image: url("./blue_texture.jpeg");
            opacity:100px;
            padding:15px;
        }
        </style>
    </head>
    <body>
        <?php

            include 'login_session.php';
            session_start();

            //if the user has not logged in
            if(!isLoggedIn())
            {
                header('Location: index.php');
                die();
            }

            //echo("Welcome:  " . $_SESSION['username'] . "!");
        ?>
        <!-- Form to enter name of the survey. -->
        <form name="new_survey" action="create_survey.php" method="post">
            <label for="title">Enter the name for this survey:</label>
            <input type="text" name="title" id="title" placeholder="Enter Title for Survey Here" required="required">     
            <input type="submit" value="Next">
        </form>
    </body>
<style>
body{
	width:1000px;
	background:#FFF;
	margin:auto;
	border-radius:5px;
	border:1px solid #000;
	padding:10px;
}
</style>
</html>	
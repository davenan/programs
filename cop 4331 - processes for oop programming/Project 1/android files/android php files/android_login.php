<?php

 	// Connect...
	require_once('android_connect.php');

	// Kill Connection...
	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

	// Request username and password from app
	$droid_user = $_REQUEST['username'];
	$droid_pass = $_REQUEST['password'];

	// If user name and pass is NULL, then nothing was entered in the app
	if($droid_user == NULL || $droid_pass == NULL){
		$response = "Enter information in the selected field";
		print(json_encode($response));
	}	

	else{
		// Check for the user in the Database
		$i = mysql_query("select * from user_accts where username= '$droid_user' and password = '$droid_pass'");
		$check = '';
		$pass = '';
		
		// This was early stages of creating the program
		// Blank if the user isn't found
		// user and password concatenated if the user is found
		// Still worked so I just left it 
		while($row = mysql_fetch_array($i)){
			$check = $row['username'];
			$pass = $check.$row['password'];
		}
		
		// Print the result
		print(json_encode($pass));
	}
	
	// Close Connection
	mysql_close($con);
?>
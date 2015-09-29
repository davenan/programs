<?php

	require_once('android_connect.php');

	// Request Username and password from app
	$droid_user = $_REQUEST['username'];
	$droid_pass = $_REQUEST['password'];

	// Blank so nothing entered from the app
	if($droid_user == NULL || $droid_pass == NULL){
		$response = "Enter information in the selected field";
		print(json_encode($response));
	}	

	else{
		
		// Check for the same user name in the DB
		$i = mysql_query("select * from user_accts where username = '$droid_user' ");
		
		// Get the result 
		$check = ''; 
		while($row = mysql_fetch_array($i)){
			$check = $row['username'];
		}
		
		// Debug
		print(json_encode($check));

		// We can create a new User account 
		if($check == NULL){
			$q = "insert into user_accts values('$droid_user', '$droid_pass')";
			$s = mysql_query($q);

			// We cant query, the user exsists already
			if(!$s){
				$response["response"] = "User Exsists already";
				print(json_encode($response));		
			}
			
			// Successful we can create
			else{
				$response["response"] = "New User Created";
				print(json_encode($response));		
			}
		}
	}
	
	// Close Connection
	mysql_close($con);
?>
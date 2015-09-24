<?php

	// Connect to Database
	require_once('android_connect.php');

	// If the connection failed just die....
	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

	// Select the Database
	mysql_select_db("android_db", $con);

	// Request user and sid from the android app
	$droid_user = $_REQUEST['user'];
	$droid_sid = $_REQUEST['sid'];
	
		// Empty check string var
		$check = '';
		
		// Query for the name of the user with a particular survey id 
		$i = mysql_query("select * from survey where user = '$droid_user' and sid = '$droid_sid'");
		
		// Fetch the possible result 
		while($row = mysql_fetch_array($i)){
			$check = $row;			
		}
		
		// Blank check means we can create that survey for the user
		if($check == NULL){
			print(json_encode('true'));
		}
		
		// No go...
		else{
			print(json_encode('false'));
		}
			
		// Close connection
		mysql_close($con);
?>

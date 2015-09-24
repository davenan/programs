<?php
	require_once('connect.php');

	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

	mysql_select_db("android_db", $con);

	$droid_user = $_REQUEST['user'];
	$droid_sid = $_REQUEST['sid'];
	
		$check = '';
		$i = mysql_query("select * from survey where user = '$droid_user' and sid = '$droid_sid'");
		
		while($row = mysql_fetch_array($i)){
			$check = $row;			
		}
		
		if($check == NULL){
			print(json_encode('true'));
		}
		
		else{
			print(json_encode('false'));
		}
			
		mysql_close($con);
?>

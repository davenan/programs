<?php
	
	// Create connection...
	require_once('android_connect.php');

	// Close if failed...
	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

		// Request sid and qnum from application
		$droid_sid = $_REQUEST['sid'];
		$droid_qnum = $_REQUEST['qnum'];
				
		// Select the survey questions in ascending order from Database
		$v = mysql_query("select * from survey where sid = '$droid_sid' order by survey.qnum asc");
		
		// Print each column with delimiters for String parsing in the app
		while($t = mysql_fetch_array($v)){
			print(json_encode($t['ques'])).":"."#";
			print(json_encode($t['ans1']))."#";
			print(json_encode($t['ans2']))."#";
			print(json_encode($t['ans3']))."#";
			print(json_encode($t['ans4']))."%";
		}
	
	// Close Connection...
	mysql_close($con);
?>
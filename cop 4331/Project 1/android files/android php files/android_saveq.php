<?php
	
	require_once('android_connect.php');

	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

	$droid_user = $_REQUEST['user'];
	$droid_sid = $_REQUEST['sid'];
	$droid_qnum = $_REQUEST['qnum'];
	$droid_ques = $_REQUEST['ques'];
	$droid_answer = $_REQUEST['answer'];
	
	
		$i = mysql_query("select * from answers where user = '$droid_user' and sid = '$droid_sid'
						  and qnum = '$droid_qnum'");
		
		$check = mysql_fetch_array($i); 
		
		if($check == NULL){
			$q = "insert into b31_16306157_db.answers(user, sid, qnum, question, answer)
			      values('$droid_user', '$droid_sid', '$droid_qnum', '$droid_ques', '$droid_answer')";
			$s = mysql_query($q);

			print(json_encode('true'));
		}
		
		else{

			$r = "update answers set answer = '$droid_answer' 
				  where user = '$droid_user' and sid = '$droid_sid'
				  and qnum = '$droid_qnum'";
				  
			$v = mysql_query($r);		  
			print(json_encode('false'));
		}
		
	mysql_close($con);
?>
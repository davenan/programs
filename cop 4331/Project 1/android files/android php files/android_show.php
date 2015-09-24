<?php
	
	$mysql_host = "sql300.byethost31.com";
	$mysql_database = "b31_16306157_db";
	$mysql_user = "b31_16306157";
	$mysql_password = "yankees1";

	$con = mysql_connect($mysql_host, $mysql_user, $mysql_password);
	mysql_select_db($mysql_database, $con);

	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}

		$droid_sid = $_REQUEST['sid'];
		$droid_qnum = $_REQUEST['qnum'];
				
		$v = mysql_query("select * from survey where sid = '$droid_sid' order by survey.qnum asc");
		
		while($row = mysql_fetch_array($v)){
		
			$count;
			$count1;
			$count2;
			$count3;
			$count4;
			
			
			
			$t = mysql_query("select * from answers where sid = '$droid_sid' order by answers.qnum asc");
			$ans = ' ';
			while($check = mysql_fetch_array($t)){
				
				$ans = $check['answer'];
			}
			
			if(strcmp($ans, $row['ans1']) == 0){
				$count1++;
			}			
		    print(json_encode($row['ques']))."#";
            print(json_encode($row['ans1']))."$count1".",";
			print(json_encode($row['ans2']))."$count2".",";
			print(json_encode($row['ans3']))."$count3".",";
			print(json_encode($row['ans4']))."$count4"."$";	
			
		}
		
		
		/*
		$t = mysql_query("select * from answers where sid = '$droid_sid' order by answers.qnum asc");
		while($check = mysql_fetch_array($t)){
			print(json_encode($check['qnum']));
			print(json_encode($check['answer']));
		}
		
		*/
		

	mysql_close($con);
?>
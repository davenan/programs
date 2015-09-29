<?php
	
	require_once('android_connect.php');
	
	if(!$con)
	{
		die("Could not establish connection:" . mysql_error());
	}
       mysql_select_db($mysql_database, $con);
	
		$i = mysql_query("select * from survey");
		while($row = mysql_fetch_array($i)){	
				$check = $output;
			    $output = $row['sid'];	
			    if($check != $output){
			       print(json_encode("$".$output));
			    }
				print(json_encode(' '));
		}
		
	mysql_close($con);
?>
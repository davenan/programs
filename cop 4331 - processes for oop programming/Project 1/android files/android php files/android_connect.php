<?php
	$mysql_host = "sql300.byethost31.com";
	$mysql_database = "b31_16306157_db";
	$mysql_user = "b31_16306157";
	$mysql_password = "yankees1";

	$con = mysql_connect($mysql_host, $mysql_user, $mysql_password);
	mysql_select_db($mysql_database, $con);
?>
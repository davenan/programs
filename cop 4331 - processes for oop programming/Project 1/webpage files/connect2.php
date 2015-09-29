<?php
	/* This is the database connection file. It is called in php scripts that need to connect to the database.*/
	DEFINE ('DB_USER', 'b31_16306157');
	DEFINE ('DB_PASSWORD', 'yankees1');
	DEFINE ('DB_HOST', 'sql300.byethost31.com');
	DEFINE ('DB_NAME', 'b31_16306157_db');

	$dbc = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);

	// Check connection
	if ($dbc->connect_error) {
	    die("Connection failed: " . $dbc->connect_error);
	} 
	//echo "Connected successfully";



?>
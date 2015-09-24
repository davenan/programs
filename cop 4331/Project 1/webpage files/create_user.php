<html>
<head>
	<title>Information gathered</title>
</head>
<body>

	<?php

		// Connect to database.

		// Friendly message.
		echo "<p>Hello World!<p>";

		// Save data sent from form into variables.
		$usern = $_POST['username'];
		$passw = $_POST['passw'];

		echo "username: " . $usern . "</br>";
		echo "password: " . $passw . "</br>";
		echo "</br>";
        
        // Connect to database.
		require_once("connect2.php");
        
        // Create query. Selects rows with the inputed username. This is part of checking if it already exists.
        $query = "SELECT username FROM user_accts WHERE username collate latin1_general_cs = '$usern'";
        
        // Query the database
        $response = $dbc->query($query);
        
        // If we got a row, then that username already exist
        if($response->num_rows > 0){
            echo '</br>that USERNAME already exists';
            header('refresh:5; url=new_user.html');
            die();
        }
        else{
            // Insert into database
            $query = $dbc->prepare("INSERT INTO user_accts (username, password) VALUES (?, ?)");
            $query->bind_param("ss", $usern, $passw);
            $query->execute();
            $query->close();
            echo '</br>Name created Success fully';
//            header('refresh:5; url=index.php');
//            die();  
        }


        // This is just code for debugging. It shows a list of all current users in db. Used to test if user was created successfully.
		// Create a query for the database.
		$query = "SELECT username, password, user_id FROM user_accts";
        $response = $dbc->query($query);

		if($response){
			echo '<table>

			<tr><th>Username</th>
			<th>Password</th>
			<th>id</th></tr>';

			while($row = $response->fetch_assoc()){
				echo '<tr><td> ' . 
				$row[username] . '</td><td>' .
				$row[password] . '</td><td>' .
				$row[user_id] . '</td>';

				echo '</tr>';
			}

			echo '</table>';
		} 
		else {
			echo "Couldnt get query";
		}

        header('refresh:5; url=index.php');
        die();

        // Debug code ends here.

		$dbc->close();
	?>

</html>
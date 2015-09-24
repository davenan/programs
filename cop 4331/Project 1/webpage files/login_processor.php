<?php
    // Includes a file with functions
    include 'login_session.php';
    
    // Start a session for the user
    session_start();

    // Connext to the data base;
    require_once("connect2.php");

    // Some random echo. Just to test if everything is ok.
    echo "<p>Hello World<p>";
    
    // Get the username and password that was  entered.
    $usern = $_POST['username'];
    $passw = $_POST['passw'];

    // Create a query. Selects rows from database that have the username $usern.
    $query = "SELECT * FROM user_accts WHERE username collate latin1_general_cs = '$usern'";

    // Query the database
    $response = $dbc->query($query);

    if($response->num_rows < 1) //no such user exists. no rows returned
    {
        // Takes you back to login page.
        header('refresh:5; url=index.php');
        echo "This username does not exist. You will be redirected back to login in a few seconds";
        die();
    }
    else{
        $userData = $response->fetch_assoc();

        if($userData['password'] != $passw) //Check if password is correct
        {
            // Takes you back to login page.
            header('refresh:5; url=index.php');
            echo "Wrong password. You will be redirected back to login in a few seconds";
            die();
        }
        else
        {
            //login successful
            // Call function in login_session.php
            validateUser($userData);
            
            // Redirect to login page.
            header('refresh:5; url=welcome.php');
            echo "Login SUCCESSFUL. You will be redirected in a few seconds";
            die();
        }
    }
     
    // Close database connection.
    $dbc->close();

?>
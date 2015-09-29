<?php

    // Used to create a session for the user that is logging in.
	function validateUser($userData)
	{
	    $_SESSION['valid'] = 1;
	    //echo $userid;
	    $_SESSION['username'] = $userData['username'];
        //$_SESSION['user_id'] = $userData['user_id'];
	    //echo $_SESSION['userid'];
	}
    
    // Checks if a user is logged in.
	function isLoggedIn()
	{
	    if(isset($_SESSION['valid']) && $_SESSION['valid'])
	        return true;
	    return false;
	}

?>
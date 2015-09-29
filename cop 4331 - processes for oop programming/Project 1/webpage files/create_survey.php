<?php
    /* This script processes the new survey name.*/
    $title = $_POST['title'];
    
    echo($title);

    require_once("connect2.php");
    
    /* This creates a query for any survey with the same name as the title that was entered */
    $query = "SELECT DISTINCT sid FROM survey WHERE sid ='$title'";
    $response = $dbc->query($query);
    
    /* If there are any responses, then that survey already exists. */
    if($response->num_rows > 0){
        echo "A survey by that title already exists. Redirecting to title creation...";
        header('refresh:5; url=new_survey.php');    
        die();
    }
    else{
        /* Sets a session variable so we can use the title of survey in the next page */
        session_start();
        $_SESSION['survey_title'] = $_POST['title'];
        /* Redirects to the next page where you add the questions. */
        header('Location: add_quest.php');
        die();  
    }
    
?>

<html>
    <head>
        <title>View Survey</title>
    </head>
    <body>
<?php
    include 'login_session.php';
    session_start();

    //if the user has not logged in
    if(!isLoggedIn())
    {
        header('Location: index.php');
        die();
    }
    
    require_once('connect2.php');
    $query = "SELECT DISTINCT sid FROM survey";
    $response = $dbc->query($query);

    echo "</br>";
    
    /* Prints out the available surveys to view and take */
    while($row = $response->fetch_assoc())
    {
        echo "<p>" . $row['sid'] . 
            " <a href='view_questions.php?survey_name=" . urlencode($row['sid']) . "'>" . "View" . "</a>" . 
            " <a href='take_survey.php?survey_name=" . urlencode($row['sid']) . "'>" . "Take Survey" . "</a>" . 
            "</p>";
    }
    $dbc->close();
?>
    </body>
</html>
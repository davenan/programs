<html>
    <head>
        <title>View a Survey</title>
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

    echo("Welcome:  " . $_SESSION['username'] . "!" . "</br>");

    $title = $_GET['survey_name'];

    require_once('connect2.php');
    echo "</br>";
    
    $query = "SELECT * FROM survey WHERE sid = '$title' order by survey.qnum asc";
    $response = $dbc->query($query);
    
    /* Loops through questions and prints them out along with its choices.*/
    while($row = $response->fetch_assoc())
    {
        echo "<strong><span>" . ($row['qnum'] + 1). ". " . $row['ques'] . "</span></strong>";
        echo "</br>";
        echo "<ol type='a'>";
        if(isset($row['ans1'])){
            echo "<li>" . $row['ans1'] . "</li>";
        }
        if(isset($row['ans2'])){
            echo "<li>" . $row['ans2'] . "</li>";
        }
        if(isset($row['ans3'])){
            echo "<li>" . $row['ans3'] . "</li>";
        }
        if(isset($row['ans4'])){
            echo "<li>" . $row['ans4'] . "</li>";
        }
        echo "</ol>";
    }

    $dbc->close();
?>
    </body>
</html>
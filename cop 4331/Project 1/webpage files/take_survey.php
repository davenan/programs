<html id='main'>
    <head>
        <title>Take a Survey</title>
<style>
        #main {
            background-image: url("./blue_texture.jpeg");
            opacity:100px;
            padding:15px;
        }
</style>

    </head>
    <body>
<?php
    
    /* This script generates html form from a survey in the database. */
    include 'login_session.php';
    session_start();

    //if the user has not logged in
    if(!isLoggedIn())
    {
        header('Location: index.php');
        die();
    }

    //echo("Welcome:  " . $_SESSION['username'] . "!" . "</br>");

    $title = $_GET['survey_name'];

    require_once('connect2.php');
    echo "</br>";

    /* Get survey from database with query. */
    $query = "SELECT * FROM survey WHERE sid = '$title'  ORDER BY qnum ASC";
    $response = $dbc->query($query);

    
    /* Creates form html */
    echo "<form name='survey' action='submit_answers.php' method='post'>";
    
    /* A hidden field to send title along with the answers */
    echo "<input type='hidden' name='title' value='" . $title . "'>";

    /* Loops through questions and creates input for them. */
    while($row = $response->fetch_assoc())
    {   
        /* Prints the question in bold */
        echo "<strong><span>" . ($row['qnum'] + 1). ". " . $row['ques'] . "</span></strong>";
        echo "</br>";
        
        /* This if-else checks to see if there any choices for the question and generates input for them if they exist */
        if(isset($row['ans1']) && $row['ans1'] != "")
        {
            echo "<label>" . "<input type='radio' name='question[" . $row['qnum'] . "]'" . " value='" . $row['ans1'] . "'" . " required='required'>" . $row['ans1'] . "</label>";
        }
        else
        {   
            /* This is for short answer which would not have any options at all. */
            echo "<input type='text' name='question[" . $row['qnum'] . "]' required='required' placeholder='Enter Answer'>";
        }
        if(isset($row['ans2']) && $row['ans2'] != "")
        {
            echo "<label>" . "<input type='radio' name='question[" . $row['qnum'] . "]'" . " value='" . $row['ans2'] . "'" . " required='required'>" . $row['ans2'] . "</label>";
        }
        if(isset($row['ans3']) && $row['ans3'] != "")
        {
            echo "<label>" . "<input type='radio' name='question[" . $row['qnum'] . "]'" . " value='" . $row['ans3'] . "'" . " required='required'>" . $row['ans3'] . "</label>";
        }
        if(isset($row['ans4']) && $row['ans4'] != "")
        {
            echo "<label>" . "<input type='radio' name='question[" . $row['qnum'] . "]'" . " value='" . $row['ans4'] . "'" . " required='required'>" . $row['ans4'] . "</label>";
        }
        echo "</br>";
    }
    /* Submit button */
    echo "<input type='submit' value='Submit Survey'>";
    echo "</form>";

    $dbc->close();
?>
    </body>
<style>
body{
	width:1000px;
	background:#FFF;
	margin:auto;
	border-radius:5px;
	border:1px solid #000;
	padding:10px;
}
</style>
</html>	
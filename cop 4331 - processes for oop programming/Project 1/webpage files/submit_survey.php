<html>
    <head>
            <title>Submit Survey</title>
    </head>
    <body>
<?php
    session_start();
    require_once('connect2.php');
        
    $questions = $_POST['question'];
    foreach($_POST['question'] as $quest)
    {
        print_r($quest);
        echo '</br>';
    }
    
    /* These are just variables for agree and disagree strings */
    $agree = 'Agree';
    $disagree = 'Disagree';
    
    /* Loops through each question */
    for($count = 0; $count < sizeof($questions); $count++)
    {
        /* Gets the question detail and save in an easy to use variable */
        $q = $questions[$count];
        
        //echo $count . "</br>";
        
        // This if else sets the query and values for the insertion into database.
        if($count >= 0 && $count < 4) // If multiple choice question
        {
            $query = $dbc->prepare("INSERT INTO survey (user, sid, qnum, ques, ans1, ans2, ans3, ans4) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
            $query->bind_param("ssssssss", $_SESSION['username'], $_SESSION['survey_title'], $count, $q['question'], $q['opt1'], $q['opt2'], $q['opt3'], $q['opt4']);
        }
        elseif($count >= 4 && $count < 8) // If agree disagree question
        {
            $query = $dbc->prepare("INSERT INTO survey (user, sid, qnum, ques, ans1, ans2) VALUES (?, ?, ?, ?, ?, ?)");
            $query->bind_param("ssssss", $_SESSION['username'], $_SESSION['survey_title'], $count, $q['question'], $agree, $disagree);
        }
        else // If short answer question
        {
            $query = $dbc->prepare("INSERT INTO survey (user, sid, qnum, ques) VALUES (?, ?, ?, ?)");
            $query->bind_param("ssss", $_SESSION['username'], $_SESSION['survey_title'], $count, $q['question']);
        }
        
        //echo "Inserted " . $count . " into the database" . "</br>";
        
        /* This line inserts into the database */
        $query->execute();
    }
    $query->close();

    /* Remove Title from session */
    unset($_SESSION['survey_title']);
    

    /* Go back to welcome page. */
    header('Location: welcome.php');
    die();
?>    
    </body>
</html>
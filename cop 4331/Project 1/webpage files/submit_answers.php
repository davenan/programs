<?php
    /* This page is for submitting answers into the database */

    /* First get the name of user thats submitting their answers. */
    session_start();
    $user = $_SESSION['username'];

    /* Connect to database */
    require_once('connect2.php');
    
    /* Get title and answers to the questions from the take survey form */
    $title = $_POST['title'];
    $q = $_POST['question'];

    /* This query is to get the details of each question such as their number and the question itself */

    $query="SELECT qnum, ques FROM survey WHERE sid = '$title'";
    $response = $dbc->query($query);

    /* This query is for inserting answers into the database using prepared statement */

    $query = $dbc->prepare("INSERT INTO answers (user, sid, qnum, question, answer) VALUES (?, ?, ?, ?, ?)");
    $query->bind_param("sssss", $user, $title, $qnum, $quest, $answer);
    
    /* Submit answers one at a time */
    while($row = $response->fetch_assoc())
    {
        $qnum = $row['qnum'];
        $quest = $row['ques'];
        $answer = $q[$qnum];
        $query->execute();
    }
    $query->close();
    $dbc->close();

    header('Location: welcome.php');
    die();
?>
<html id='main'>
    <head>
        <title>Add Questions</title>
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

    include 'login_session.php';
    session_start();

    //if the user has not logged in
    if(!isLoggedIn())
    {
        header('Location: index.php');
        die();
    }
    
    /* Some debugging code to make sure everything is working correctly*/
    //echo("Welcome:  " . $_SESSION['username'] . "!");
    echo(" The title of this survey is: " . $_SESSION['survey_title']);

?>
        </br>
        <!-- This is the form to create a survey. -->
        <form name="new_survey" action="submit_survey.php" method="post">
            
            <h2>Multiple Choice</h2>
            <!-- Questions 1 - 4 are multiple choice -->
            <label for="question1">Enter Question 1:</label>
            <input type="text" name="question[0][question]" id="question1" required="required">
            <input type="text" name="question[0][opt1]" id="q1opt1" placeholder="Option 1" required="required">
            <input type="text" name="question[0][opt2]" id="q1opt2" placeholder="Option 2" required="required"> 
            <input type="text" name="question[0][opt3]" id="q1opt3" placeholder="Option 3" required="required"> 
            <input type="text" name="question[0][opt4]" id="q1opt4" placeholder="Option 4" required="required">
            </br>
            
            <label for="question2">Enter Question 2:</label>
            <input type="text" name="question[1][question]" id="question2" required="required">
            <input type="text" name="question[1][opt1]" id="q2opt1" placeholder="Option 1" required="required">
            <input type="text" name="question[1][opt2]" id="q2opt2" placeholder="Option 2" required="required"> 
            <input type="text" name="question[1][opt3]" id="q2opt3" placeholder="Option 3" required="required"> 
            <input type="text" name="question[1][opt4]" id="q2opt4" placeholder="Option 4" required="required"> 
            </br>

            <label for="question3">Enter Question 3:</label>
            <input type="text" name="question[2][question]" id="question3" required="required">
            <input type="text" name="question[2][opt1]" id="q3opt1" placeholder="Option 1" required="required">
            <input type="text" name="question[2][opt2]" id="q3opt2" placeholder="Option 2" required="required"> 
            <input type="text" name="question[2][opt3]" id="q3opt3" placeholder="Option 3" required="required"> 
            <input type="text" name="question[2][opt4]" id="q3opt4" placeholder="Option 4" required="required"> 
            </br>

            <label for="question4">Enter Question 4:</label>
            <input type="text" name="question[3][question]" id="question4" required="required">
            <input type="text" name="question[3][opt1]" id="q4opt1" placeholder="Option 1" required="required">
            <input type="text" name="question[3][opt2]" id="q4opt2" placeholder="Option 2" required="required">
            <input type="text" name="question[3][opt3]" id="q4opt3" placeholder="Option 3" required="required"> 
            <input type="text" name="question[3][opt4]" id="q4opt4" placeholder="Option 4" required="required">
            </br>

            <h2>Agree/Disagree</h2>
            <!-- Questions 5 - 8 are Agree/Disagree. The choices get filled in automatically. -->
            <label for="question5">Enter Question 5:</label>
            <input type="text" name="question[4][question]" id="question5" required="required">
            </br>
            
            <label for="question6">Enter Question 6:</label>
            <input type="text" name="question[5][question]" id="question6" required="required">
            </br>

            <label for="question7">Enter Question 7:</label>
            <input type="text" name="question[6][question]" id="question7" required="required">
            </br>

            <label for="question8">Enter Question 8:</label>
            <input type="text" name="question[7][question]" id="question8" required="required">
            </br>

            <h2>Short Answer</h2>
            <!-- Questions 9 - 10 are short answer. -->
            <label for="question9">Enter Question 9:</label>
            <input type="text" name="question[8][question]" id="question9" required="required">
            </br>
            
            <label for="question10">Enter Question 10:</label>
            <input type="text" name="question[9][question]" id="question10" required="required">
            </br>
            
            <!-- Button to submit the form. -->
            <input type="submit" value="Submit">
        </form>
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
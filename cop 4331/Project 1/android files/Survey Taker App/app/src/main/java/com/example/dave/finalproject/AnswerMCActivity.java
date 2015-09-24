package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import java.util.ArrayList;


// This class is the entry point to answering all the survey questions
public class AnswerMCActivity extends ActionBarActivity {

    // Holds all of the questions passed from TakeSurveyConnector
    String parts[];

    // Question ID number
    int qid = 0;

    // Attributes for UI of a Multiple choice question
    TextView txtQuestion;
    RadioButton rda, rdb, rdc, rdd;
    Button butNext;

    // Our user and survey id
    String user, sid;

    // Custom Arraylist to store our answers
    ArrayList<QuestionsHelper> save_answers = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.answer_mc);

        // User and sid are passed from take survey connector -> store it in globals
        user = getIntent().getExtras().getString("user");
        sid = getIntent().getExtras().getString("survey");

        // Layout attributes
        txtQuestion=(TextView)findViewById(R.id.textView1);
        rda=(RadioButton)findViewById(R.id.radio0);
        rdb=(RadioButton)findViewById(R.id.radio1);
        rdc=(RadioButton)findViewById(R.id.radio2);
        rdd=(RadioButton)findViewById(R.id.radio3);

        // Grab the questions passed in from TakeSurveyConnector
        String result = getIntent().getExtras().getString("result");

        // We seperate questions from each other and store in String[]
        // Format ->   (Question: ans1, ans2, ans3, ans4 %)
        parts = result.split("%");

        // Generate the first MC question and display on UI
        setQuestionView();

        // Next button on the UI
        butNext=(Button)findViewById(R.id.button1);
        butNext.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                RadioGroup grp = (RadioGroup) findViewById(R.id.radioGroup1);
                RadioButton answer = (RadioButton) findViewById(grp.getCheckedRadioButtonId());

                // The question id for these values are MC Questions, just keep on same activity
                if(qid == 0 || qid == 1 || qid == 2 || qid == 3) {

                    // Get the text from the radio button from the selected answer
                    String s = answer.getText().toString();

                    // Add the answer to our Arraylist
                    save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                          txtQuestion.getText().toString(), s));

                    // Get next question
                    setQuestionView();

                    // Clear the radio button from prev. question
                    grp.clearCheck();

                }

                // We hit the else case on qid = 4, we are gonna start Agree/Disagree questions now....
                else if(qid == 4){

                    // First save question 3 answer
                    String s = answer.getText().toString();
                    save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                     txtQuestion.getText().toString(), s));


                    // Loop through our saved answers, make one HTTPpost request to save to DB
                    // Uses an asynchronous task to keep the UI interactive
                    for (int i = 0; i < save_answers.size(); i++){
                        new SaveQuestionAnswers().doInBackground(save_answers.get(i).user,
                                save_answers.get(i).sid, save_answers.get(i).qnum, save_answers.get(i).question,
                                save_answers.get(i).answer);
                    }

                    // Then start Agree/Disagree activity
                    Intent i = new Intent(AnswerMCActivity.this, AnswerADActivity.class);

                    // Transfer our String[] of questions to Agree/Disagree activity
                    // Along with the user and the survey name
                    i.putExtra("parts", parts);
                    i.putExtra("user", user);
                    i.putExtra("survey", sid);

                    // Start next activity, close this one.
                    startActivity(i);

                 }
            }
        });
    }


    // This method renders the questions and answers to the UI
    private void setQuestionView()
    {
        // Grab string from array at index qid
        String everything = parts[qid];

        // Questions are seperated by a ":" delimiter in string
        String seperate[] = everything.split(":");

        // This is the question ONLY
        String question = seperate[0];

        // This is the 4 answers
        String ans = seperate[1];

        // The answers use "#" delimiter
        String answers[] = ans.split("#");

        // These hold just answers 1-4
        String ans1 = answers[1];
        String ans2 = answers[2];
        String ans3 = answers[3];
        String ans4 = answers[4];

        // At last we set each attribute of UI to all the stuff
        txtQuestion.setText(question);
        rda.setText(ans1);
        rdb.setText(ans2);
        rdc.setText(ans3);
        rdd.setText(ans4);

        // Here we increment our global counter
        qid++;
    }
}

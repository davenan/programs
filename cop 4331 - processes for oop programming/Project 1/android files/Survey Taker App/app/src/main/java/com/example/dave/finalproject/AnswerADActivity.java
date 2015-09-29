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


// This class continues the take survey process from AnswerMC
// Executes the same way as the AnswerMC class does
// only difference is the layout which is why I used another class
// Didn't know how to reuse a layout
public class AnswerADActivity extends ActionBarActivity {

    String parts[];
    int qid = 4;
    TextView txtQuestion;
    RadioButton rda, rdb;
    Button butNext;
    String user, sid;

    ArrayList<QuestionsHelper> save_answers = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.answer_ad);


         // Here we grab our String[] of questions passed from AnswerMC
         // We also grab the user and survey passed from AnswerMC
         parts = getIntent().getExtras().getStringArray("parts");
         user = getIntent().getExtras().getString("user");
         sid = getIntent().getExtras().getString("survey");


        // Set each UI attribute by ID
        txtQuestion = (TextView) findViewById(R.id.textView1);
        rda = (RadioButton) findViewById(R.id.radio0);
        rdb = (RadioButton) findViewById(R.id.radio1);
        butNext = (Button) findViewById(R.id.button1);

        // Generate the first AD Question
        setQuestionView();

        butNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                RadioGroup grp = (RadioGroup) findViewById(R.id.radioGroup1);
                RadioButton answer = (RadioButton) findViewById(grp.getCheckedRadioButtonId());

                // The question id for these values are AD Questions, just keep on same activity if qid = val
                if(qid == 4 || qid == 5 || qid == 6 || qid == 7) {

                    // Get the text from the radio button selected in answer
                    String s = answer.getText().toString();

                    // Send to save question answers -> This saves to our DB
                   // new SaveQuestionAnswers(AnswerADActivity.this).execute(user, sid, String.valueOf(qid - 1), txtQuestion.getText().toString(), s);
                   save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                        txtQuestion.getText().toString(), s));

                    // Get next question
                    setQuestionView();

                    grp.clearCheck();
                }

                // We hit the else case on qid = 7, we are gonna start Short Answer questions now....
                else{

                    // First save question 7 answer
                    String s = answer.getText().toString();
                    save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                          txtQuestion.getText().toString(), s));

                    // Iterate through each 4 AD answers and save to DB, one HTTP call
                    for (int i = 0; i < save_answers.size(); i++){
                       new SaveQuestionAnswers().doInBackground(save_answers.get(i).user,
                                save_answers.get(i).sid, save_answers.get(i).qnum, save_answers.get(i).question,
                                save_answers.get(i).answer);
                    }

                    // Transfer our String[] of questions to Short answer activity
                    // Along with the user and the survey name
                    Intent i = new Intent(getApplicationContext(), AnswerSAActivity.class);
                    i.putExtra("parts", parts);
                    i.putExtra("user", user);
                    i.putExtra("survey", sid);

                    // Start next activity, close this one.
                    startActivity(i);
                    finish();
                }

            }
        });
    }


    // Sets the UI to the Question
    private void setQuestionView() {

        // Grab string from array at index qid
        String everything = parts[qid];

        // Questions are seperated by a ":" delimiter in string
        String seperate[] = everything.split(":");

        // This is the question ONLY
        String question = seperate[0];

        // We only have two choices, so just hardcode it
        txtQuestion.setText(question);
        rda.setText("Agree");
        rdb.setText("Disagree");
        qid++;
    }
}

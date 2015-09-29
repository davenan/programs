package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.ArrayList;


/* Same except concept as AnswerMC and AnswerAD, only difference
 * the user us writing the answer instead */
public class AnswerSAActivity extends ActionBarActivity {

    String parts[];
    int qid = 8;
    TextView txtQuestion;

    EditText writeAns;
    Button butNext;
    String user, sid;

    ArrayList<QuestionsHelper> save_answers = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.answer_sa);

        parts = getIntent().getExtras().getStringArray("parts");
        txtQuestion = (TextView) findViewById(R.id.textView1);
        butNext = (Button)findViewById(R.id.button15);
        writeAns = (EditText)findViewById(R.id.editText2);

        user = getIntent().getExtras().getString("user");
        sid = getIntent().getExtras().getString("survey");


        setQuestionView();
        butNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(qid == 8 || qid == 9) {

                    String s = writeAns.getText().toString();
                    writeAns.getText().clear();

                    save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                         txtQuestion.getText().toString(), s));

                    setQuestionView();
                }

                else if(qid == 10){
                    String s = writeAns.getText().toString();
                    save_answers.add(new QuestionsHelper(user, sid, String.valueOf(qid - 1),
                                         txtQuestion.getText().toString(), s));

                    for (int i = 0; i < save_answers.size(); i++){
                       new SaveQuestionAnswers().execute(save_answers.get(i).user,
                                save_answers.get(i).sid, save_answers.get(i).qnum, save_answers.get(i).question,
                                save_answers.get(i).answer);
                    }

                    Intent i = new Intent(getApplicationContext(), MainMenuActivity.class);
                    i.putExtra("user", user);
                    startActivity(i);
                    finish();
                }
            }
        });
    }

    private void setQuestionView() {
        String everything = parts[qid];
        String parts[] = everything.split(":");
        txtQuestion.setText(parts[0]);
        qid++;
    }
}

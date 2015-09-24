package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import java.io.InputStream;
import java.util.ArrayList;


public class MultipleChoiceActivity extends ActionBarActivity {

    String user_sid;
    int question_num;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.create_mc);

        // Pressing this button creates a multiple choice question
        // So this activity CREATES a multiple choice question
        Button b1 = (Button)findViewById(R.id.mc_save);
        b1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // Grab the User and the Survey ID (Single String)
                user_sid = getIntent().getExtras().getString("user_sid");

                // Split it into two strings (s1->User, s2->SID)
                String[]parts = user_sid.split(":");

                // Set variables from split string above ^
                String u_name = parts[0];
                String sid_name = parts[1];

                // This was passed from previous activity which is Q #
                question_num = getIntent().getExtras().getInt("qnum");

                // The question, and answers entered on UI by user
                EditText e = (EditText) findViewById(R.id.mc_question);
                EditText e1 = (EditText) findViewById(R.id.choice1);
                EditText e2 = (EditText) findViewById(R.id.choice2);
                EditText e3 = (EditText) findViewById(R.id.choice3);
                EditText e4 = (EditText) findViewById(R.id.choice4);
                String ques = e.getText().toString();
                String choice1 = e1.getText().toString();
                String choice2 = e2.getText().toString();
                String choice3 = e3.getText().toString();
                String choice4 = e4.getText().toString();

                // Send to PHP file for processing
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("user", u_name));
                nameValuePairs.add(new BasicNameValuePair("sid", sid_name));
                nameValuePairs.add(new BasicNameValuePair("qnum", Integer.toString(question_num)));
                nameValuePairs.add(new BasicNameValuePair("ques", ques));
                nameValuePairs.add(new BasicNameValuePair("ans1", choice1));
                nameValuePairs.add(new BasicNameValuePair("ans2", choice2));
                nameValuePairs.add(new BasicNameValuePair("ans3", choice3));
                nameValuePairs.add(new BasicNameValuePair("ans4", choice4));

                // Connect to Database
                DBConnector db = new DBConnector();

                // Send the data over using -> (4) MULTIPLE.PHP
                InputStream is = db.create_connection(nameValuePairs, 4);


                // User never entered a question or one of the four choices
                if(ques.isEmpty() || choice1.isEmpty() || choice2.isEmpty() || choice4.isEmpty()){

                    if(ques.isEmpty()) {
                        Toast.makeText(getApplicationContext(), "Enter a Question", Toast.LENGTH_LONG).show();
                    }
                    else {
                        Toast.makeText(getApplicationContext(), "Question must have four choices", Toast.LENGTH_LONG).show();
                    }
                }

                // If everything was entered then proceed...
                else {
                    // Go back to create survey activity, for user to create another question
                    Intent i = new Intent(getApplicationContext(), CreateSurveyActivity.class);
                    i.putExtra("user_sid", user_sid);
                    startActivity(i);
                }
            }
        });
    }
}

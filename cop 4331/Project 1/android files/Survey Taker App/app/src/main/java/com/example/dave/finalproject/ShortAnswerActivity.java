package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import java.io.InputStream;
import java.util.ArrayList;


public class ShortAnswerActivity extends ActionBarActivity {

    String user_sid;
    int question_num;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.create_sa);


        // This button creates a Short Answer Question
        Button b1 = (Button)findViewById(R.id.sa_save_button);
        b1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // User and Survey ID string
                user_sid = getIntent().getExtras().getString("user_sid");

                // Seperate User and SID
                String[]p = user_sid.split(":");

                // Store user and sid into Strings
                String u_name = p[0].toString();
                String sid_name = p[1].toString();

                // Get the question number from create survey
                question_num = getIntent().getExtras().getInt("qnum");

                // Create a question from edit text input by user
                EditText e = (EditText)findViewById(R.id.sa_question);
                String ques = e.getText().toString();


                // Send all variables to PHP for processing
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("user", u_name));
                nameValuePairs.add(new BasicNameValuePair("sid", sid_name));
                nameValuePairs.add(new BasicNameValuePair("qnum", Integer.toString(question_num)));
                nameValuePairs.add(new BasicNameValuePair("ques", ques));


                // Open connection to DB -> uses (4) MULTIPLE.PHP
                DBConnector db = new DBConnector();
                InputStream is = db.create_connection(nameValuePairs, 4);

                // Go back to Create Survey for more questions
                Intent i = new Intent(getApplicationContext(), CreateSurveyActivity.class);
                i.putExtra("user_sid", user_sid);
                startActivity(i);
            }
        });
    }
}

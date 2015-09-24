package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class CreateSurveyActivity extends ActionBarActivity {

    /*
       This activity sets up a list view where user clicks on an option
       on the list to create the question. Questions 1-4 are ONLY Multiple
       Choice, Questions 5-8 are only Agree and Disagree, and Questions 9-10
       are only for Short Answer Activity. When user clicks an option on the list,
       the activity MultipleChoice, AgreeDisagree, ShortAnswer Activity is opened.
     */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.create_screen);


        // List View for the User Interface to Create Questions
        final ListView list = (ListView) findViewById(R.id.create_survey_list);

        // Strings to populate the List Interface
        String[] values = new String[]{
                "Create Multiple Choice Question 1",
                "Create Multiple Choice Question 2",
                "Create Multiple Choice Question 3",
                "Create Multiple Choice Question 4",
                "Create Agree/Disagree Question 5",
                "Create Agree/Disagree Question 6",
                "Create Agree/Disagree Question 7",
                "Create Agree/Disagree Question 8",
                "Create Short Answer Question 9",
                "Create Short Answer Question 10",
                "Save Your Survey",
        };

        // Define a new adapter for the list
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_list_item_1, android.R.id.text1, values);

        // Set the adapter
        list.setAdapter(adapter);

        // Listen to the user click for each list item
        list.setOnItemClickListener(new ListView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {


                // Remember this is the (USER:SURVEY) String
                String user_sid = getIntent().getExtras().getString("user_sid");

                // Seperate USER and SURVEY by colon delimiter
                String parts[] = user_sid.split(":");

                // Get user from user_sid string for choice 10
                String u_name = parts[0];

                // Our index to the user clicked item
                int question = position;

                /*
                    Index 0-3 = Multiple Choice Question
                    Index 4-7 = Agree Disagree Question
                    Index 8,9 = Short Answer Question
                    We pass the index to the list which is the question number
                    We pass the (USER:SURVEY) string to next activity
                    and we start the respective activity
                */
                if (question == 0 || question == 1 || question == 2 || question == 3) {
                    Intent i = new Intent(getApplicationContext(), MultipleChoiceActivity.class);
                    i.putExtra("user_sid", user_sid);
                    i.putExtra("qnum", question);
                    startActivity(i);
                }

                if(question == 4 || question == 5 || question == 6 || question == 7){
                    Intent i = new Intent(getApplicationContext(), AgreeDisagreeActivity.class);
                    i.putExtra("user_sid", user_sid);
                    i.putExtra("qnum", question);
                    startActivity(i);
                }

                if(question == 8 || question == 9) {
                    Intent i = new Intent(getApplicationContext(), ShortAnswerActivity.class);
                    i.putExtra("user_sid", user_sid);
                    i.putExtra("qnum", question);
                    startActivity(i);
                }

                // This is the SAVE YOUR SURVEY option in the List
                // Save the questions and we return to the Main Menu class
                // User is not logged out so pass back the user to the Main menu as well
                if(question == 10){
                    Intent i = new Intent(getApplicationContext(), MainMenuActivity.class);
                    i.putExtra("user", u_name);
                    startActivity(i);
                }
            }
        });
    }
}

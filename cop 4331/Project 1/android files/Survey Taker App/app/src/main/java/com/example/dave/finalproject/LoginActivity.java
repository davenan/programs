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


public class LoginActivity extends ActionBarActivity {

    String droid_user, droid_pass;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);

        /* Actions related to the user pressing the Login Button */
        /* Button Listener */
        Button login = (Button) findViewById(R.id.login_button);
        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // The Username and Password the user enters
                EditText editText = (EditText) findViewById(R.id.username_login);
                EditText editText1 = (EditText) findViewById(R.id.password_login);
                droid_user = editText.getText().toString();
                droid_pass = editText1.getText().toString();

                // Connect to Database
                DBConnector db = new DBConnector();

                // Setup credentials for PHP file
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("username",droid_user));
                nameValuePairs.add(new BasicNameValuePair("password",droid_pass));

                // Once open send the made up username and password uses -> REGISTER.PHP(2)
                InputStream is = db.create_connection(nameValuePairs, 1);

                // Our query result from the PHP file
                String result = db.get_response(is);

                // Did not type something in either field
                if (droid_user.isEmpty() || droid_pass.isEmpty()) {
                    Toast.makeText(getApplicationContext(), "Enter a Username/Password", Toast.LENGTH_SHORT).show();
                }

                // Check database for user and see first if user and then pass was correct
                else if (result.contains(droid_user)) {
                    if (result.contains(droid_pass)) {

                        // If so its the user was found -> Start Main Menu Activity
                        Toast.makeText(getApplicationContext(), "Login Success", Toast.LENGTH_SHORT).show();
                        Intent i = new Intent(getApplicationContext(), MainMenuActivity.class);

                        // Pass the user to next intent
                        i.putExtra("user", droid_user);
                        startActivity(i);
                    }
                }

                // Problem Logging in the user
                else {
                    // No such user exsists in the database
                    Toast.makeText(getApplicationContext(), "Username/Pass Incorrect", Toast.LENGTH_SHORT).show();
                }
            }
        });

        // Sign Up button takes the user to the Register Activity to create a name/pass
        Button signup = (Button) findViewById(R.id.register_button);
        signup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Just start the registration activity
                Intent i = new Intent(getApplicationContext(), RegisterActivity.class);
                startActivity(i);
            }
        });
    }
 }




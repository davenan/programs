package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.os.StrictMode;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import org.apache.http.NameValuePair;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.StringTokenizer;


public class ShowSurveyConnector extends ActionBarActivity {

    // List view for populating all surveys
    ListView listval;

    // Adapter to populate list view
    ArrayAdapter<String> adapter;

    // Holds end values.
    ArrayList<String> list = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.show_connector);
        final StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);


        // Create the list view
        listval = (ListView)findViewById(R.id.take_survey_list);

        // Set the list adapter
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, android.R.id.text1, list);

        // Nothing to send, instead we just query all rows directly from PHP file
        ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();

        // Connect to the database
        DBConnector db = new DBConnector();

        // Grab JSON Object
        InputStream is = db.create_connection(nameValuePairs, 5);

        // Set JSON Object to result
        String result = db.get_response(is);

        // Remove all quotations -> PHP file also places a (" ") delimiter in string
        // This is to sep
        result = result.replace("\"", "");

        // Tokenize string by the spaces
        StringTokenizer st2 = new StringTokenizer(result, "$");

        // Clean up
        while (st2.hasMoreElements()) {
            list.add(st2.nextToken());
        }

        removeDuplicates(list);
        listval.setAdapter(adapter);
        listval.setOnItemClickListener(new ListView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id){

                String user = getUser();
                Intent i = new Intent(getApplicationContext(), ShowSurveyResults.class);
                i.putExtra("survey", list.get(position));
                i.putExtra("user", user);
                finish();
                startActivity(i);
            }

        });

        Log.e("log_tag", "String returned " + result);

    }



        @Override
        public boolean onCreateOptionsMenu (Menu menu){
            // Inflate the menu; this adds items to the action bar if it is present.
            getMenuInflater().inflate(R.menu.menu_show_survey_connector, menu);
            return true;
        }

        @Override
        public boolean onOptionsItemSelected (MenuItem item){
            // Handle action bar item clicks here. The action bar will
            // automatically handle clicks on the Home/Up button, so long
            // as you specify a parent activity in AndroidManifest.xml.
            int id = item.getItemId();

            //noinspection SimplifiableIfStatement
            if (id == R.id.action_settings) {
                return true;
            }

            return super.onOptionsItemSelected(item);
        }

    public String getUser() {
        String str = null;
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            str = extras.getString("user");
        }
        return str;
    }

    private void removeDuplicates(ArrayList<String> list) {
        int count = list.size();
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                if (list.get(i).equals(list.get(j))) {
                    list.remove(j--);
                    count--;
                }
            }
        }
    }
}

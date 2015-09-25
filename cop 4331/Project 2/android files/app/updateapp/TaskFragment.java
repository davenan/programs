package com.example.mc.myapplication;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
public class TaskFragment extends Fragment{
    private TaskCallbacks mCallbacks;
    private taskFrag mTask;
    private boolean mRunning=false;
    Context context;
    String Response;
     String params;
    static interface TaskCallbacks {
        public void onPreExecute();
        public void onProgressUpdate(int percent);
        public void onCancelled();
        public void onPostExecute();
    }
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        mCallbacks = (TaskCallbacks) activity;
    }
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRetainInstance(true);

    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        cancel();
    }
    public void start(String param) {
        if (!mRunning) {
            params=param;
            mTask = new taskFrag();
            mTask.execute();
            mRunning = true;
        }
    }
    public void cancel() {
        if (mRunning) {
            mTask.cancel(false);
            mTask = null;
            mRunning = false;
        }
    }
    public boolean isRunning() {
        return mRunning;
    }
    private class taskFrag extends AsyncTask<Void, Void, Void> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mCallbacks.onPreExecute();
            mRunning = true;
        }

        @Override
        protected Void doInBackground(Void... arg0) {
            Response="fail";
            try{
                URL url=new URL("http://project.bbr.ms/phoneapp.php");
                HttpURLConnection con=(HttpURLConnection) url.openConnection();
                con.setRequestMethod("POST");
                con.setDoOutput(true);
                OutputStream os = con.getOutputStream();
                os.write(params.getBytes());
                os.flush();
                os.close();
                BufferedReader incomming=new BufferedReader(new InputStreamReader(con.getInputStream()));
                StringBuilder result = new StringBuilder();
                String line;
                Response="";
                while((line = incomming.readLine()) != null)
                    Response+=line;
            }catch(Exception e){Response=e.getMessage();}

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            mCallbacks.onPostExecute();
            mRunning = false;
        }
    }
}
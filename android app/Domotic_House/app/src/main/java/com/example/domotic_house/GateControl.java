package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;


public class GateControl extends AppCompatActivity {

private TextView titleGate=null;
private static ImageButton play_Btn;
private static ImageButton pause_Btn;
public Button home_gate;

void languageReset(){
titleGate=findViewById(R.id.title_gate);
if(Communication.language=="ita") {
    titleGate.setText("PANNELLO DI CONTROLLO CANCELLO");
}else{
    titleGate.setText("GATE CONTROL PANNEL");
}
}

//method to create and start the timer; when the timer ends it call the receiving method and reset it
public void startTimer(){
    Communication.countDownTimer=new CountDownTimer(Communication.receivingTimer, 1000) {
        @Override
        public void onTick(long l) {
            Communication.receivingTimer=l;
        }

        @Override
        public void onFinish() {
            getValuesFromSpreadsheet();
            timerReset();
        }
    }.start();
}
//method to reset the timer
public void timerReset(){
    if(Communication.countDownTimer!=null) {
        Communication.countDownTimer.cancel();
        Communication.receivingTimer=3000;
        startTimer();
    }
    else{startTimer();
    }
}

public static void matchingGate(){
    if (Communication.GMaster.getMoveGate()=="close" || Communication.GMaster.getMoveGate()=="open") {
        play_Btn.setVisibility(View.VISIBLE);
        pause_Btn.setVisibility(View.INVISIBLE);
    }else{
        play_Btn.setVisibility(View.INVISIBLE);
        pause_Btn.setVisibility(View.VISIBLE);
    }
}


//method to set values to spreadsheet
public void getValuesFromSpreadsheet() {
    String url=Communication.URL+"read";
    StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
        @Override
        public void onResponse(String response) {
            try {
                Communication.retryTimes=0;
                Communication.insertValues(response, 'g');
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Toast.makeText(getApplicationContext(),"Connection ERROR", Toast.LENGTH_SHORT ).show();
                    Communication.retryTimes++;
                    if(Communication.retryTimes>5){
                        Communication.retryTimes=0;
                        Toast.makeText(getApplicationContext(),"Too many connection ERRORs,\nCheck your connection, then try again", Toast.LENGTH_SHORT ).show();
                        home_gate.callOnClick();
                        }
                    }
            });
    int socketTimeOut=5000;
    RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
    stringRequest.setRetryPolicy(retryPolicy);
    RequestQueue queue= Volley.newRequestQueue(this);
    queue.add(stringRequest);
}

//method to get values from spreadsheet
public void setValuetoSpreadsheet(String value) {
    StringRequest stringRequest=new StringRequest(Request.Method.GET, Communication.createUrl(value), new Response.Listener<String>() {
        @Override
        public void onResponse(String response) {
        }
    },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                }
            });
    int socketTimeOut=5000;
    RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
    stringRequest.setRetryPolicy(retryPolicy);
    RequestQueue queue= Volley.newRequestQueue(this);
    queue.add(stringRequest);
    timerReset();
}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gate_control);
        play_Btn=findViewById(R.id.play_gate);
        play_Btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Communication.GMaster.getMoveGate()=="close"){
                    play_Btn.setVisibility(View.INVISIBLE);
                    pause_Btn.setVisibility(View.VISIBLE);
                    Communication.GMaster.setMoveGate(Gmove.opening);
                }
                if(Communication.GMaster.getMoveGate()=="open"){
                    play_Btn.setVisibility(View.INVISIBLE);
                    pause_Btn.setVisibility(View.VISIBLE);
                    Communication.GMaster.setMoveGate(Gmove.closing);
                }
                setValuetoSpreadsheet("Gate="+Communication.GMaster.getMoveGate());
            }
        });
        pause_Btn=findViewById(R.id.pause_gate);
        pause_Btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Communication.GMaster.getMoveGate()=="opening"){
                    play_Btn.setVisibility(View.VISIBLE);
                    pause_Btn.setVisibility(View.INVISIBLE);
                    Communication.GMaster.setMoveGate(Gmove.open);
                }
                if(Communication.GMaster.getMoveGate()=="closing"){
                    play_Btn.setVisibility(View.VISIBLE);
                    pause_Btn.setVisibility(View.INVISIBLE);
                    Communication.GMaster.setMoveGate(Gmove.close);
                }
                setValuetoSpreadsheet("Gate="+Communication.GMaster.getMoveGate());
            }
        });
        home_gate=findViewById(R.id.home_gate);
        home_gate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Communication.countDownTimer.cancel();
                Communication.receivingTimer=3000;
                startActivity(new Intent(GateControl.this, MainActivity.class));
            }
        });
    getValuesFromSpreadsheet();
    timerReset();

    matchingGate();
    languageReset();
    }
}
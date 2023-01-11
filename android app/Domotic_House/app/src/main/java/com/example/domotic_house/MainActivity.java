package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.app.PendingIntent;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
public Button lightPage;
public Button gatePage;
public Button thermostatPage;
public Switch language_btn;
TextView title;
void languageReset(){
    title=findViewById(R.id.title_home);
    if(Communication.language=="ita") {
        title.setText("CASA DOMOTICA");
        lightPage.setText("LUCI");
        gatePage.setText("CANCELLO");
        thermostatPage.setText("TERMOSTATO");
        if(language_btn.isChecked()) language_btn.toggle();
    }else{
        title.setText("DOMOTIC HOUSE");
        lightPage.setText("LIGHTS");
        gatePage.setText("GATE");
        thermostatPage.setText("THERMOSTAT");

        if(!language_btn.isChecked()) language_btn.toggle();
    }
}
    int i=0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lightPage = findViewById(R.id.light_page);
        gatePage = findViewById(R.id.gate_page);
        thermostatPage = findViewById(R.id.thermostat_page);

        Communication.dataExample();
        lightPage.setOnClickListener(new View.OnClickListener() {
        @Override
            public void onClick(View view) {
            startActivity(new Intent(MainActivity.this,LightsControl.class));
        }
        });
        thermostatPage.setOnClickListener(new View.OnClickListener() {
        @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this,ThermostatControl.class));
            }
        });
        gatePage.setOnClickListener(new View.OnClickListener() {
        @Override
            public void onClick(View view) {
            startActivity(new Intent(MainActivity.this,GateControl.class));
            }
        });
        language_btn=findViewById(R.id.language);
        language_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Communication.language=="eng") Communication.language="ita";
                    else Communication.language="eng";
                languageReset();
            }
        });
    languageReset();
    }
}
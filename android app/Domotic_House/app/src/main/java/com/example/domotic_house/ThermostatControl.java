package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.ToggleButton;

public class ThermostatControl extends AppCompatActivity {
public Button home_therm;
public static Button modeT;
public static ToggleButton power_therm;
public Button plus_therm;
public Button minus_therm;
private static TextView tempText = null;
private ImageView fire;
private ImageView ice;
TextView title;

void languageReset(){
        title=findViewById(R.id.thermostat_title);
        if(Communication.language=="ita") {
            title.setText("PANNELLO DI CONTROLLO TERMOSTATO");
        }else{
            title.setText("THERMOSTAT CONTROL PANNEL");
        }
    }
public void refreshThermostat(){
    if ( (Thermostat.onTherm==Ton_off.valueOf("on")) && (!ThermostatControl.power_therm.isChecked()) )
        ThermostatControl.power_therm.toggle();
    if ( (Thermostat.onTherm==Ton_off.valueOf("off")) && (ThermostatControl.power_therm.isChecked()) )
        ThermostatControl.power_therm.toggle();
    tempTextRefresh();
    if(Thermostat.onTherm==Ton_off.valueOf("on")) {
        if (Thermostat.modeTherm == Tmode.warm) {
            fire.setVisibility(View.VISIBLE);
            ice.setVisibility(View.INVISIBLE);
        } else {
            fire.setVisibility(View.INVISIBLE);
            ice.setVisibility(View.VISIBLE);
        }
    }else{
        fire.setVisibility(View.INVISIBLE);
        ice.setVisibility(View.INVISIBLE);
    }
}

public static void tempTextRefresh(){
    tempText.setText("Temp: "+Thermostat.tempImp);
}

public void setTempImp(int i) {
    if(i!=Thermostat.tempImp)
        Communication.changes='y';  //tell the background process that there has been a change
    Thermostat.tempImp=i;
    tempText.setText("Temp: "+Thermostat.tempImp);
    //comunicate the new value of temp
}

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_thermostat_control);
        tempText=findViewById(R.id.tempText);
        setTempImp(Thermostat.tempImp);
        fire=findViewById(R.id.fire);
        ice=findViewById(R.id.ice);
        power_therm=findViewById(R.id.on_off);
        power_therm.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(power_therm.isChecked())
                    Thermostat.onTherm=Ton_off.valueOf("on");
                else Thermostat.onTherm=Ton_off.valueOf("off");
                Communication.changes='y';
                Communication.synchronizeData();
                refreshThermostat();
            }
        });

        plus_therm=findViewById(R.id.plus_therm);
        plus_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setTempImp(Thermostat.tempImp+1);
            }
        });

        minus_therm=findViewById(R.id.minus_therm);
        minus_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setTempImp(Thermostat.tempImp-1);
            }
        });

        home_therm=findViewById(R.id.home_therm);
        home_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(ThermostatControl.this, MainActivity.class));
            }
        });

        modeT=findViewById(R.id.mode_therm);
        modeT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(power_therm.isChecked())
                if(Thermostat.modeTherm==Tmode.valueOf("warm")) Thermostat.modeTherm=Tmode.valueOf("cool");
                    else Thermostat.modeTherm=Tmode.valueOf("warm");
                refreshThermostat();
            }
        });
        languageReset();
        refreshThermostat();
    }
}
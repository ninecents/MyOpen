package com.ninecents;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    Button button_member_method;
    Button button_static_method;
    Button button_overload_method;
    static MainActivity _this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 绑定事件
        button_member_method = (Button) findViewById(R.id.button_member_method);
        button_member_method.setOnClickListener(this);
        button_static_method = (Button) findViewById(R.id.button_static_method);
        button_static_method.setOnClickListener(this);
        button_overload_method = (Button) findViewById(R.id.button_overload_method);
        button_overload_method.setOnClickListener(this);
        _this = this;
    }

    private int _member_method(String text) {
        Toast.makeText(getApplicationContext(),text,Toast.LENGTH_SHORT).show();

        // 返回值为0：展示红色，否则展示蓝色
        return 0;
    }

    private static void _static_method() {
        Toast.makeText(MainActivity._this,"button_static_method",Toast.LENGTH_SHORT).show();
    }

    private void _overload_method(String text) {
        Toast.makeText(getApplicationContext(),text,Toast.LENGTH_SHORT).show();
    }

    private void _overload_method(int val) {
        String text;
        if (val == 1) {
            text = "111111111";
        } else {
            text = "222222222";
        }
        Toast.makeText(getApplicationContext(),text,Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.button_member_method:
                int ret = _member_method("这是button_member_method");
                if (ret == 0) {
                    button_member_method.setBackgroundColor(Color.parseColor("#FF0000"));
                } else {
                    button_member_method.setBackgroundColor(Color.parseColor("#0000FF"));
                }
                break;
            case R.id.button_static_method:
                MainActivity._static_method();
                break;
            case R.id.button_overload_method:
                _overload_method(1);
                break;
        }
    }
}
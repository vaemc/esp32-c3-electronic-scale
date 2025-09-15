package com.vaemc.electronic_scale_app;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;


import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.net.URISyntaxException;

public class MainActivity extends AppCompatActivity {

    private RelativeLayout main;
    private EditText etIp;
    private Button btnConnect;
    private TextView tvWeight;
    private WebSocketClient webSocketClient;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
        initView();

        btnConnect.setOnClickListener(v -> {
            String ip = etIp.getText().toString();

            if (ip.isEmpty() || ip.isBlank()) {
                Toast.makeText(this, "请输入IP", Toast.LENGTH_SHORT).show();

            } else {
                String ws = "ws://" + ip + ":81";
                URI uri  = null;
                try {
                    uri = new URI(ws);
                } catch (URISyntaxException e) {
                    e.printStackTrace();
                }


                webSocketClient = new WebSocketClient(uri) {
                    @Override
                    public void onOpen(ServerHandshake handshakedata) {

                        MainActivity.this.runOnUiThread(() -> {
                            Toast.makeText(MainActivity.this, "连接成功!", Toast.LENGTH_SHORT).show();


                        });
                    }

                    @Override
                    public void onMessage(String message) {
                        MainActivity.this.runOnUiThread(() -> {
                           tvWeight.setText(message+"克");
                        });
                    }

                    @Override
                    public void onClose(int code, String reason, boolean remote) {

                    }

                    @Override
                    public void onError(Exception ex) {

                    }
                };
                webSocketClient.connect();



            }
        });
    }

    private void initView() {
        main = (RelativeLayout) findViewById(R.id.main);
        etIp = (EditText) findViewById(R.id.et_ip);
        btnConnect = (Button) findViewById(R.id.btn_connect);
        tvWeight = (TextView) findViewById(R.id.tv_weight);
    }
}
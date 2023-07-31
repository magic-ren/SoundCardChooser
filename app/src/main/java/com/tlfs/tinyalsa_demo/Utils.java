package com.tlfs.tinyalsa_demo;

import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;

/**
 * @Author rendedong
 * @Date 2023/7/31
 */
public class Utils {
    public static String getCurrentTime() {
        Date now = new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String formattedDateTime = formatter.format(now);
        return formattedDateTime;
    }
}

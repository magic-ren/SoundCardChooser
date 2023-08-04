package com.tlfs.tinyalsa_demo;

import android.text.TextUtils;

import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;

/**
 * @Author rendedong
 * @Date 2023/7/31
 * 工具类
 */
class Utils {

    /**
     * 获取当前时间，格式“yyyyMMdd_HHmmss”
     * 例如：20230804_112123
     **/
    protected static String getCurrentTime() {
        Date now = new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String formattedDateTime = formatter.format(now);
        return formattedDateTime;
    }

    /**
     * 依据key获取当前系统的属性值。
     * 没有此属性时返回null。
     **/
    private static String getCustomPropertyStr(String key) {
        try {
            Class<?> systemPropertiesClass = Class.forName("android.os.SystemProperties");
            Method getMethod = systemPropertiesClass.getMethod("get", String.class);
            return (String) getMethod.invoke(null, key);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    protected static int getCustomProperty(String key) {
        int cardMode;
        String customPropertyValue = getCustomPropertyStr(key);
        if (TextUtils.isEmpty(customPropertyValue)) {
            cardMode = -1;
        } else {
            cardMode = Integer.parseInt(customPropertyValue);
        }
        return cardMode;
    }
}

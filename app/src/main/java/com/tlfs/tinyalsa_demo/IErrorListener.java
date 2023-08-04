package com.tlfs.tinyalsa_demo;

/**
 * @Author rendedong
 * @Date 2023/8/3
 * C层的错误回调
 */
public interface IErrorListener {
    void onError(String msg);
}

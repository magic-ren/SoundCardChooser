package com.rdd.player;

/**
 * @Author rendedong
 * @Date 2023/8/3
 * 播放器接口
 */
public interface IPlayer {

    /**
     * 初始化播放器
     **/
    void prepare();

    /**
     * 开始播放。
     * 说明：只有在“未播放状态”和“播放完成状态”下可以调用，否则提示错误，如下：
     * STATUS_PLAYING，提示错误“正在播放”；
     * STATUS_PAUSE，提示错误“播放暂停中，请使用“继续”功能”；
     * STATUS_UNINIT，提示错误“播放器未初始化”；
     **/
    void start();

    /**
     * 暂停播放
     **/
    void pause();

    /**
     * 继续播放
     **/
    void continuee();

    /**
     * 重置播放器
     **/
    void reset();

    /**
     * 完成播放。
     * 说明：保存之前的音频，并跳转到指定页面。
     **/
    void complete();

    /**
     * 获取播放器的状态。
     * Player.STATUS_UNINIT：未初始化播放器
     * Player.STATUS_UNPLAY：未开始播放或者已重置
     * Player.STATUS_PLAYING：正在播放
     * Player.STATUS_PAUSE：已暂停
     * Player.STATUS_COMPLETE：已完成
     **/
    int getStatus();

    /**
     * 释放播放器的资源
     **/
    void release();

}

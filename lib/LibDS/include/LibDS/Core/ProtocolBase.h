/*
 * Copyright (c) 2015 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once
#ifndef _LIB_DS_PROTOCOL_H
#define _LIB_DS_PROTOCOL_H

#include <QBitArray>
#include <QHostInfo>
#include <QTcpSocket>
#include <QNetworkReply>

#include "LibDS/Core/Common.h"
#include "LibDS/Core/Watchdog.h"
#include "LibDS/Core/Discovery/Discovery.h"

/**
 * \class DS_Protocol
 *
 * Implements an abstract class to be used as a base for any protocol that
 * will be used to drive an FRC robot.
 */
class LIB_DS_DECL DS_ProtocolBase : public QObject {
    Q_OBJECT

  public:
    explicit DS_ProtocolBase();
    ~DS_ProtocolBase();

    /**
     * Returns the current team number
     */
    int team() const;

    /**
     * The current status code of the robot
     */
    int status() const;

    /**
     * Returns \c true if the user code is loaded on the robot
     */
    bool robotCode() const;

    /**
     * The number of packets that we sent
     */
    int sentPackets() const;

    /**
     * Returns \c true if the robot communications are working
     */
    bool isConnected() const;

    /**
     * Returns \c true if we are sending the date/time to the robot
     */
    bool sendDateTime() const;

    /**
     * Returns the current alliance of the robot
     */
    DS_Alliance alliance() const;

    /**
     * Returns the current control mode of the robot
     */
    DS_ControlMode controlMode() const;

    /**
     * Returns the current communication status
     */
    DS_CommunicationStatus communicationStatus() const;

    /**
     * A list with the input data of the joysticks. This is just a reference
     * to the joystick list in the protocol manager.
     */
    QList<DS_Joystick*>* joysticks() const;

    /**
     * Returns the default radio address or the custom radio address
     */
    QString radioAddress();

    /**
     * Returns the default robot address or the custom robot address.
     *
     * If there is no custom address set, the function may return the robot
     * address or the fallback address (10.XX.YY.2)
     */
    QString robotAddress();

    /**
     * Constructs a client packet and updates the internal values
     */
    QByteArray getClientPacket();

    /**
     * Returns the port in which we send data to the robot
     *
     * \note This function must be implemented by each protocol
     */
    virtual int robotPort() = 0;

    /**
     * Returns the port in which we receive robot data
     *
     * \note This function must be implemented by each protocol
     */
    virtual int clientPort() = 0;

    /**
     * Returns the default radio address
     *
     * \note This function must be implemented by each protocol
     */
    virtual QString defaultRadioAddress() = 0;

    /**
     * Returns the default robot address
     *
     * \note This function must be implemented by each protocol
     */
    virtual QString defaultRobotAddress() = 0;

  public slots:
    /**
     * Resets the internal values of the protocol and emits the appropiate
     * signals when the robot communication is lost.
     *
     * After doing that, the function will try to ping the robot again
     */
    void reset();

    /**
     * Changes the team number of the robot, this can be used to generate
     * the robot and radio address.
     */
    void setTeamNumber (int team);

    /**
     * Changes the robot address to \a address
     */
    void setRobotAddress (QString address);

    /**
     * Updates the \a alliance of the robot
     */
    void setAlliance (DS_Alliance alliance);

    /**
     * Updates the control \a mode of the robot
     */
    void setControlMode (DS_ControlMode mode);

    /**
     * Indicates the protocol from where to read input from the \a joysticks
     */
    void setJoysticks (QList<DS_Joystick*>* joysticks);

    /**
     * Reads the robot data and calls the appropiate functions to interpret it
     */
    void readRobotPacket (QByteArray data);

    /**
     * Reboots the robot
     *
     * \note This function must be implemented by each protocol
     */
    virtual void reboot() = 0;

    /**
     * Restarts the robot code or the user program
     *
     * \note This function must be implemented by each protocol
     */
    virtual void restartCode() = 0;

  signals:
    /**
     * Emitted when the protocol detects that the status of the
     * user code has changed
     */
    void codeChanged (bool);

    /**
     * Emitted when the state of the network communications with the robot
     * has been changed. Unlike the other signals with a \c bool value, this
     * signal contains more information about the communication status, such
     * as:
     *     - The robot responds ping requests, but does not respond to DS
     *     - The robot responds to ping requests and DS
     *     - The robot does not respond to ping requests nor the DS
     */
    void communicationsChanged (DS_CommunicationStatus);

    /**
     * Emitted when the protocol detects that the robot voltage has changed
     */
    void voltageChanged (QString);

    /**
     * Emitted when the protocol detects that the RAM usage of the robot
     * has changed
     */
    void ramUsageChanged (int);

    /**
     * Emitted when the protocol detects that the disk usage of the robot
     * has changed
     */
    void diskUsageChanged (int);

    /**
     * Emitted when the protocol detects that the control mode has changed.
     * Note: this signal can be emitted when the user changes the control mode
     * or when the robot itself changes its mode (e.g. e-stop)
     */
    void controlModeChanged (DS_ControlMode);

    /**
     * Emitted when the client has just connected to the robot and downloaded
     * its library INI files and analyzed them
     */
    void libVersionChanged (QString);

    /**
     * Emitted when the client has just connected to the robot and downloaded
     * its PCM INI files and analyzed them
     */
    void rioVersionChanged (QString);

    /**
     * Emitted when the client has just connected to the robot and downloaded
     * its PDP information and analyzed them
     */
    void pdpVersionChanged (QString);

    /**
     * Emitted when the client has just connected to the robot and downloaded
     * the PCM information files and analyzed them
     */
    void pcmVersionChanged (QString);

    /**
     * Emitted when the Driver Station changes the robot address or the team number
     * Can be used internally or externally to update network addresses.
     */
    void robotAddressChanged (QString);

    /**
     * Emitted when we receive a packet from the robot. Used by the library
     * to generate and send another control packet to the robot.
     */
    void packetReceived();

  protected slots:
    /**
     * Implements the necessary steps to reset the internal values of a protocol
     *
     * \note This function must be implemented by each protocol
     */
    virtual void resetProtocol() = 0;

    /**
     * Implements a method to get more information about the robot components
     *
     * \note This function must be implemented by each protocol
     */
    virtual void downloadRobotInformation() = 0;

    /**
     * Interprets the received robot \a data and emits the appropiate signals
     *
     * \note This function must be implemented by each protocol
     */
    virtual void readRobotData (QByteArray data) = 0;

  protected:
    /**
     * Uses the joystick input information to generate a data array to be
     * sent along the client packet
     *
     * \note This function must be implemented by each protocol
     */
    virtual QByteArray generateJoystickData() = 0;

    /**
     * Generates the neccesary data to send the robot the current timezone
     * of the client DS
     *
     * \note This function must be implemented by each protocol
     */
    virtual QByteArray generateTimezoneData() = 0;

    /**
     * Generates a control packet to be sent to the robot
     *
     * \note This function must be implemented by each protocol
     */
    virtual QByteArray generateClientPacket() = 0;

    /**
     * Returns a 'generic' \c DS_ControlMode \c enum by reading
     * the 'raw' value of the given \a mode.
     *
     * \note This function must be implemented by each protocol
     */
    virtual DS_ControlMode getControlMode (int mode) = 0;

    /**
     * Returns the control code used by the protocol to represent the selected
     * control \a mode
     *
     * \note This function must be implemented by each protocol
     */
    virtual int getControlCode (DS_ControlMode mode) = 0;

    /**
     * Returns the code used by the protocol to represent the selected
     * \a alliance
     *
     * \note This function must be implemented by each protocol
     */
    virtual int getAllianceCode (DS_Alliance alliance) = 0;

    /**
     * Calculates the size of the \a joystick.
     * This function is implemented for each protocol to meet the
     * specific requirements of the protocol in question.
     *
     * \note This function must be implemented by each protocol
     */
    virtual int getJoystickSize (DS_Joystick* joystick) = 0;

    /**
     * Changes the current robot \a status
     */
    void updateStatus (int status);

    /**
     * Changes the state of the robot code and emits the appropiate signals
     */
    void updateRobotCode (bool available);

    /**
     * Changes the state of the send date/time variable
     */
    void updateSendDateTime (bool sendDT);

    /**
     * Changes the state of the communications and emits the appropiate signals
     */
    void updateCommunications (DS_CommunicationStatus status);

    /**
     * 'Calculcates' the voltage from the values of the \a major and \a minor
     * parameters and emits the appropiate signals
     */
    void updateVoltage (int major, int minor);

  private:
    /**
     * This is the team number, you may use it for a variety of purposes
     * This variable is changed with the \c setTeamNumber() function.
     */
    int m_team;

    /**
     * Represents the operation status of the robot.
     * This value can be used to send 'special' commands to the robot,
     * such as rebooting the main controller (e.g. roboRIO or cRIO) or restarting
     * the user code
     */
    int m_status;

    /**
     * Holds the number of times the \c reset() function was called.
     * We use it to toggle the usage of the fallback robot address every certain
     * number of protocol resets.
     */
    int m_resetCount;

    /**
     * Represents the number of packets sent to the robot
     */
    int m_sentPackets;

    /**
     * This variable should be set to \c true when the user code is loaded
     */
    bool m_robotCode;

    /**
     * If set to \c true, the protocol will send data to the robot radio
     * instead of the robot address.
     *
     * This value is toggled automatically on each robot reset
     */
    bool m_useFallbackAddress;

    /**
     * Holds the communication status of the robot
     */
    DS_CommunicationStatus m_communicationStatus;

    /**
     * If set to \c true, you should send the current date time data
     * with the \c generateTimezoneData() function
     */
    bool m_sendDateTime;

    /**
     * The IP address of the robot, calculate it with the \c QHostInfo class
     */
    QString m_robotIp;

    /**
     * The network address of the robot component (e.g. cRIO or roboRIO)
     * This variable is overwritten with the \c setRobotAddress() function.
     */
    QString m_robotAddress;

    /**
     * The radio address of the the robot (generally 10.XX.YY.1)
     */
    QString m_radioAddress;

    /**
     * The current alliance of the robot.
     * This variable is changed with the \c setAlliance() function.
     */
    DS_Alliance m_alliance;

    /**
     * The watchdog, used to reset internal values and refresh data when
     * robot is not present or does not respond
     */
    DS_Watchdog m_watchdog;

    /**
     * Finds out the IP of the robot address in a cross-platform way
     */
    NetworkDiscovery m_discovery;

    /**
     * The current control mode of the robot.
     * This variable is changed with the \c setControlMode() function.
     */
    DS_ControlMode m_controlMode;

    /**
     * A list with the input data of the joysticks. This is just a reference
     * to the joystick list in the protocol manager.
     */
    QList<DS_Joystick*>* m_joysticks;

    /**
     * Used for pinging the robot
     */
    QTcpSocket m_pingSocket;

  private slots:
    /**
     * Pings the robot using a TCP socket
     */
    void pingRobot();

    /**
     * Changes the address of the robot to the robot's IP and then tries to
     * ping the robot to get the connection status
     */
    void onIpFound (QString address, QString ip);

    /**
     * Called when the connection state between the robot TCP connection and
     * the client is changed
     */
    void onStateChanged (QAbstractSocket::SocketState state);
};

#endif

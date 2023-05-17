#include <DBASEV/communication.h>

void * thread_func2(void *arg)
{
    
    int cnt = 0;
    MsgBuf msg;
    msg.msgtype = 1;
    key_t key = 5656;
    int key_id = mq_init(key);

    struct msqid_ds buf;
    while(1){
        msg.value = ++cnt;
        if (cnt >= 10) {
            cout << "Message Sending Finished!" << endl;
            break;
        }
        strcpy(msg.buf, "37.8 1 6");
        push(key_id,buf, msg);
        //usleep(1);
        //cout << "value: " << msg.value << endl;
    }

    return 0;

}

void * sender(void *arg)
{
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }

    serialSetting(serial_port);

    // Create a Mavlink instance
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    //mavlink_channel_t channel = MAVLINK_COMM_0;
    mavlink_statustext_t statustext;

    // Set the fields of the statustext message
    statustext.severity = MAV_SEVERITY_INFO; // Set severity to INFO

    const int max_chunk_size = 49;
    char temp_buffer[50];
    std::string temp;
    
    while (true) {
		std::string message = "37.8 31 6";
		
		
		for (int i = 0; i < message.length(); i += max_chunk_size) {
				int chunk_size = std::min(max_chunk_size, static_cast<int>(message.length() - i));
				temp = message.substr(i, chunk_size);

				// Copy temp to buffer
				temp.copy(temp_buffer, chunk_size);
				temp_buffer[chunk_size] = '\0';

				strcpy(statustext.text, temp_buffer); // Set the message text

				// Encode the statustext message
				mavlink_msg_statustext_encode(255, 200, &msg, &statustext);

				// Convert the message to a byte buffer
				uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

				// Send the message over the serial port
				ssize_t bytesWritten = write(serial_port, buf, len);
				
				if (bytesWritten < 0 || bytesWritten != len) {
					std::cerr << "Error sending message." << std::endl;
					close(serial_port);
					//return 1;
				}
		}
    }

    // Close the serial port
    close(serial_port);

    return 0;
}

void * receiver(void *arg)
{
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Error opening serial port." << std::endl;
        //return 1;
    }
    sender_serialSetting(fd);
    receiver_serialSetting(fd);

    //Create a Mavlink instance
    mavlink_message_t msg;
    //uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    mavlink_status_t status;
    mavlink_channel_t channel = MAVLINK_COMM_0;
    std::string message;
    
    //Wait for messages
    while (true) {
        printf("start while\n");
        
        bool message_received = receiveMessage(fd, msg, status, channel, message);
        
        if (message_received) {
            printf("%s\n",message.c_str());
        } else {
            message = "ERR";
            printf("%s\n",message.c_str());
            printf("timeout occurred\n");
        }
    }
    return 0;
}


# Attacking scanf() 
In this task, scanf() function was attacked in the following code (secret.c). The password the user entered was stolen and was sent to remote server.
The library that secret.c needed was replaced to a new library. In the new library, scanf() function gets a password from the user but he doesn't know that the function also sends the password to server.

![Screenshot 2023-04-19 201618](https://user-images.githubusercontent.com/117903915/233154218-ce5aa90e-1d88-499c-a618-52455d181430.png)

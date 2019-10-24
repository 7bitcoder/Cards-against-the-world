# Cards-against-the-world
# How it works :)
Desktop aplication is build with c++/sfml and connects with c++/winsocket server</br>
backend is multithread server, each thread represend game lobby and handles things like game states, executing commands, exchanging data between players, checking if players are still connecteg etc. Each lobby has it own thread which handles unicode game chat. </br>
Server is simply secured you need to send specific code to get access. </br>
To run server code i used Amazon AWS it provides free cloud computing, within you can set up small windows server. </br>
Currently there is only one deck available "Talia Rocka PL" in future i plan to create separate server that can send to clients many decks
Amazon AWS </br>
https://aws.amazon.com/free/?all-free-tier.sort-by=item.additionalFields.SortRank&all-free-tier.sort-order=asc
Sfml https://www.sfml-dev.org/index.php

# ChessMatch
## Client 
Client application is written with QtWidgets. Network part is written using Boost.Asio library. In order to play, every client needs to be registered and logged in. While playing, user can choose one of four color schemes for board cells: Classic, Green, Blue and Red.

## Server
Server is a multi-threaded application, written using Boost.Asio. Server stores thread-safe queues of messages from clients, and messages with results of completed games. Each queue is processing in it's own thread. GameManager object is responsible for selection of opponents, game creation and processes game messages.

### Sql database
To store user data and game results, PostgreSQL is used. To setup required database 'chessmatch' and 'users' and 'games' tables run next line in terminal:
<br />``` psql -U "$POSTGRES_USER" < SetupFile.sql ```<br /> <br />
'games' table stores nicknames of both players, total number of moves they made, game result and game duration in seconds.
### How to build and run
To build and run Server application on Linux, run next lines in terminal in the Server application's source code folder:
```
CMake -S. -B build 
cmake --build build
./build/ChessMatchServer
```

### Elo rating
To range players Elo rating is used. To get the expected number of score points user 'A' gets in a game with player 'B' used formula: $$E_A=\dfrac{1}{1 + 10^\frac{R_A-R_B}{400}}$$ 
where $R_A$ is 'A' player rating before game, $R_B$ is 'B' player rating before game.</br >
New Elo rating is calculated using this formula: $$R_A^\prime=R_A+K*(S_A-E_A)$$
<br />

| $\mathbf{S_A}$ value | Conditions |
| :---: | :---: |
| $$1$$ | Player 'A' won |
| $$0.5$$ | Game ended in a draw |
| $$0$$ | Player 'A' lost |

| $\mathbf{K}$ value | Conditions |
| :---: | :---: |
| $$40$$ | 'A' player rating < 2400 and number of played games < 30 |
| $$20$$ | 'A' player rating < 2400 and number of played game >= 30 |
| $$10$$ | 'A' player rating >= 2400 |

In this application player's rating can't become lower than 0. Top 100 players are shown in Client application's profile page, sorted by rating descending.

## UML diagrams 
UML class diagram for the Server application was created in draw.io app. Corresponding files in .drawio and .PDF formats are in the 'UML diagrams' folder.

![Server](https://github.com/EL96cpp/ChessMatch/assets/120955824/896befb4-5f36-473e-b6e5-bef47eba3e06)



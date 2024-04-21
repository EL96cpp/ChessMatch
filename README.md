# ChessMatch
## Client 
Client application is written with QtWidgets. Network part is written using Boost.Asio library. In order to play, every client needs to be registered and logged in. While playing, user can choose one of four color schemes for board cells: Classic, Green, Blue and Red.

## Server
Server is a multi-threaded application, written using Boost.Asio. Server stores thread-safe queues of messages from clients, and messages with results of completed games. Each queue is processing in it's own thread. GameManager object is responsible for selection of opponents, game creation and processes game messages.

### Sql database
To store user data and game results, PostgreSQL is used. To setup required database 'chessmatch' and 'users' and 'games' tables run next line in terminal:
<br />``` psql -U "$POSTGRES_USER" < SetupFile.sql ```<br />

### Elo rating
To range players Elo rating is used. To get the expected number of score point user 'A' gets in a game with player 'B' used formula: $$E_A=\dfrac{1}{1 + 10^\frac{R_A-R_B}{400}}$$ 
New Elo rating is calculated using this formula: $$R_A^\prime=R_A+K*(S_A-E_A)$$
Where $$R_A$$ and $$R_B$$ are A player and B player ratings before game correspondingly. 

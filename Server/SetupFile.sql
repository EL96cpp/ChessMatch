CREATE DATABASE chessmatch; 
\c chessmatch; 
CREATE TABLE users (nickname VARCHAR(30), password VARCHAR(30), rating INT, games_played INT);
CREATE TABLE games (white_player VARCHAR(30), black_player VARCHAR(30), winner VARCHAR(30), number_of_moves INT, total_time INT); 

-- MySQL dump 10.15  Distrib 10.0.17-MariaDB, for Linux (x86_64)
--
-- Host: pavelow.eng.uah.edu    Database: LocoLayout
-- ------------------------------------------------------
-- Server version	5.0.95-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Not dumping tablespaces as no INFORMATION_SCHEMA.FILES table on this server
--

--
-- Table structure for table `DS_Connectivity`
--

DROP TABLE IF EXISTS `DS_Connectivity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `DS_Connectivity` (
  `Current` text,
  `NumberOfConnections` int(11) default NULL,
  `Conn1` text,
  `Conn2` text,
  `Conn3` text
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DS_Connectivity`
--

LOCK TABLES `DS_Connectivity` WRITE;
/*!40000 ALTER TABLE `DS_Connectivity` DISABLE KEYS */;
INSERT INTO `DS_Connectivity` VALUES ('1_15',2,'4_13','1_8','NULL'),('1_14',2,'1_16','1_8','NULL'),('1_16',2,'1_14','4_14','NULL'),('4_14',2,'1_16','4_15','NULL'),('4_15',3,'4_14','4_16','1_12'),('1_12',2,'4_15','1_9','NULL'),('1_9',1,'1_12','NULL','NULL'),('4_16',2,'4_15','1_10','NULL'),('1_10',2,'4_16','3_14','NULL'),('3_14',2,'1_10','3_15','NULL'),('3_13',3,'1_11','3_8','3_15'),('1_11',2,'3_13','4_13','NULL'),('4_13',2,'1_11','1_15','NULL'),('1_8',3,'1_7','1_14','1_15'),('3_15',3,'3_16','3_14','3_13'),('3_16',3,'3_15','1_3','1_6'),('1_3',2,'3_16','4_4','NULL'),('1_7',3,'1_8','4_3','4_4'),('4_3',2,'1_7','6_5','NULL'),('4_4',3,'6_6','1_3','1_7'),('6_6',2,'4_4','8_13','NULL'),('1_6',3,'3_16','3_4','1_5'),('2_3',3,'2_4','2_2','2_1'),('2_2',2,'2_3','1_1','NULL'),('1_1',2,'2_2','2_1','NULL'),('2_1',2,'1_1','2_3','NULL'),('2_4',2,'2_3','2_5','NULL'),('2_5',3,'2_4','2_7','2_6'),('2_7',2,'2_5','2_9','NULL'),('2_6',2,'2_5','2_8','NULL'),('2_8',2,'2_6','2_10','NULL'),('2_9',2,'2_7','2_11','NULL'),('2_10',2,'2_8','2_12','NULL'),('2_11',2,'2_9','2_12','NULL'),('2_12',3,'2_13','2_11','2_10'),('6_5',2,'4_3','8_14','NULL'),('1_5',3,'1_6','3_3','4_2'),('3_4',2,'1_6','6_7','NULL'),('8_14',2,'6_5','8_9','NULL'),('8_13',2,'6_6','8_16','NULL'),('6_7',2,'3_4','6_2','NULL'),('6_2',2,'6_7','8_15','NULL'),('4_2',3,'1_5','3_1','5_8'),('5_8',3,'4_2','5_5','5_7'),('5_7',3,'5_8','5_6','7_1'),('3_3',2,'1_5','3_2','NULL'),('3_2',2,'3_3','6_3','NULL'),('6_3',2,'3_2','6_1','NULL'),('3_1',2,'4_2','6_8','NULL'),('6_8',2,'3_1','6_4','NULL'),('6_4',2,'6_8','6_1','NULL'),('5_5',2,'5_8','7_4','NULL'),('5_6',2,'5_7','7_3','NULL'),('7_1',2,'5_7','7_2','NULL'),('7_2',1,'7_1','NULL','NULL'),('7_3',1,'5_6','NULL','NULL'),('7_4',1,'5_5','NULL','NULL'),('8_9',3,'8_10','8_14','8_16'),('8_16',3,'8_13','1_4','8_9'),('8_15',3,'8_4','6_2','6_1'),('6_1',3,'8_15','6_3','6_4'),('3_8',2,'3_13','3_6','NULL'),('3_6',3,'3_8','5_16','4_11'),('4_11',2,'3_6','4_10','NULL'),('4_10',2,'4_11','5_1','NULL'),('5_1',2,'4_10','5_13','NULL'),('5_16',2,'3_6','5_15','NULL'),('5_15',2,'5_16','5_14','NULL'),('5_14',2,'5_15','5_3','NULL'),('5_3',3,'5_4','5_14','5_13'),('5_13',3,'5_3','5_1','5_2'),('5_2',3,'5_13','4_9','4_12'),('4_9',2,'5_2','3_7','NULL'),('4_12',2,'5_2','3_5','NULL'),('3_5',1,'4_12','NULL','NULL'),('3_7',1,'4_9','NULL','NULL'),('5_4',2,'5_3','8_1','NULL'),('2_13',2,'2_12','2_14','NULL'),('8_4',3,'8_3','1_4','8_15'),('1_4',2,'8_4','8_16','NULL'),('2_14',3,'2_13','2_15','2_16'),('8_3',3,'8_4','8_2','8_1'),('8_1',3,'8_6','5_4','8_3'),('8_10',3,'8_9','8_11','8_12'),('8_2',2,'8_3','8_7','NULL'),('8_6',2,'8_1','7_6','NULL'),('8_7',2,'8_2','7_8','NULL'),('7_8',2,'8_7','7_5','NULL'),('7_5',3,'7_7','7_8','8_5'),('8_5',2,'7_5','8_8','NULL'),('8_8',1,'8_5','NULL','NULL'),('7_6',2,'8_6','8_12','NULL'),('7_7',2,'7_5','7_13','NULL'),('7_13',2,'7_7','8_11','NULL'),('8_11',2,'7_13','8_10','NULL'),('8_12',2,'7_6','8_10','NULL'),('2_15',2,'2_14','1_2','NULL'),('2_16',2,'2_14','1_2','NULL'),('1_2',2,'2_16','2_15','NULL');
/*!40000 ALTER TABLE `DS_Connectivity` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-04-28 12:50:53

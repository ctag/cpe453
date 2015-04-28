-- MySQL dump 10.15  Distrib 10.0.17-MariaDB, for Linux (x86_64)
--
-- Host: pavelow.eng.uah.edu    Database: cpe453
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
-- Table structure for table `track_power`
--

DROP TABLE IF EXISTS `track_power`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `track_power` (
  `ds` varchar(10) NOT NULL,
  `state` tinyint(1) NOT NULL,
  `color` tinytext NOT NULL,
  PRIMARY KEY  (`ds`),
  UNIQUE KEY `ds_UNIQUE` (`ds`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `track_power`
--

LOCK TABLES `track_power` WRITE;
/*!40000 ALTER TABLE `track_power` DISABLE KEYS */;
INSERT INTO `track_power` VALUES ('1-3',1,'yellow'),('1-2',1,'yellow'),('2-13',1,'yellow'),('2-12',1,'yellow'),('2-11',1,'yellow'),('2-10',1,'yellow'),('2-16',1,'yellow'),('2-15',1,'yellow'),('2-14',1,'yellow'),('2-9',1,'yellow'),('2-8',1,'yellow'),('1-1',1,'yellow'),('2-5',1,'yellow'),('2-4',1,'yellow'),('2-7',1,'yellow'),('2-6',1,'yellow'),('2-1',1,'yellow'),('2-3',1,'yellow'),('2-2',1,'yellow'),('1-4',1,'yellow'),('1-5',1,'yellow'),('1-6',1,'yellow'),('1-7',1,'yellow'),('1-8',1,'yellow'),('1-9',1,'yellow'),('1-10',1,'yellow'),('1-11',1,'yellow'),('1-12',1,'yellow'),('1-13',1,'yellow'),('1-14',1,'yellow'),('1-15',1,'yellow'),('1-16',1,'yellow'),('3-1',1,'yellow'),('3-2',1,'yellow'),('3-3',1,'yellow'),('3-4',1,'yellow'),('3-5',1,'yellow'),('3-6',1,'yellow'),('3-7',1,'yellow'),('3-8',1,'yellow'),('3-9',1,'yellow'),('3-10',1,'yellow'),('3-11',1,'yellow'),('3-12',1,'yellow'),('3-13',1,'yellow'),('3-14',1,'yellow'),('3-15',1,'yellow'),('3-16',1,'yellow'),('4-1',1,'yellow'),('4-2',1,'yellow'),('4-3',1,'yellow'),('4-4',1,'yellow'),('4-5',1,'yellow'),('4-6',1,'yellow'),('4-7',1,'yellow'),('4-8',1,'yellow'),('4-9',1,'yellow'),('4-10',1,'yellow'),('4-11',1,'yellow'),('4-12',1,'yellow'),('4-13',1,'yellow'),('4-14',1,'yellow'),('4-15',1,'yellow'),('4-16',1,'yellow'),('5-1',1,'yellow'),('5-2',1,'yellow'),('5-3',1,'yellow'),('5-4',1,'yellow'),('5-5',1,'yellow'),('5-6',1,'yellow'),('5-7',1,'yellow'),('5-8',1,'yellow'),('5-9',1,'yellow'),('5-10',1,'yellow'),('5-11',1,'yellow'),('5-12',1,'yellow'),('5-13',1,'yellow'),('5-14',1,'yellow'),('5-15',1,'yellow'),('5-16',1,'yellow'),('6-1',0,'yellow'),('6-2',0,'yellow'),('6-3',0,'yellow'),('6-4',0,'yellow'),('6-5',0,'yellow'),('6-6',0,'yellow'),('6-7',0,'yellow'),('6-8',0,'yellow'),('6-9',0,'yellow'),('6-10',0,'yellow'),('6-11',0,'yellow'),('6-12',1,'yellow'),('6-13',1,'yellow'),('6-14',1,'yellow'),('6-15',1,'yellow'),('6-16',1,'yellow'),('7-1',1,'yellow'),('7-2',1,'yellow'),('7-3',1,'yellow'),('7-4',1,'yellow'),('7-5',1,'yellow'),('7-6',1,'yellow'),('7-7',1,'yellow'),('7-8',1,'yellow'),('7-9',1,'yellow'),('7-10',1,'yellow'),('7-11',1,'yellow'),('7-12',1,'yellow'),('7-13',1,'yellow'),('7-14',1,'yellow'),('7-15',1,'yellow'),('7-16',1,'yellow'),('8-1',1,'yellow'),('8-2',1,'yellow'),('8-3',1,'yellow'),('8-4',1,'yellow'),('8-5',1,'yellow'),('8-6',1,'yellow'),('8-7',1,'yellow'),('8-8',1,'yellow'),('8-9',1,'yellow'),('8-10',1,'yellow'),('8-11',1,'yellow'),('8-12',1,'yellow'),('8-13',1,'yellow'),('8-14',1,'yellow'),('8-15',1,'yellow'),('8-16',1,'yellow');
/*!40000 ALTER TABLE `track_power` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-04-28 12:50:36

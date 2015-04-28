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
-- Table structure for table `7_6`
--

DROP TABLE IF EXISTS `7_6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `7_6` (
  `X` float default NULL,
  `Y` float default NULL,
  `TrackType` int(11) default NULL,
  `Node` text
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `7_6`
--

LOCK TABLES `7_6` WRITE;
/*!40000 ALTER TABLE `7_6` DISABLE KEYS */;
INSERT INTO `7_6` VALUES (107,38,21,'A'),(107,37,2,'A'),(107,36,2,'A'),(107,35,2,'A'),(107,34,2,'A'),(107,33,2,'A'),(107,32,2,'A'),(107,31,2,'A'),(107,30,2,'A'),(107,29,2,'A'),(107,28,2,'A'),(107,27,2,'A'),(107,26,2,'A'),(107,25,2,'A'),(107,24,2,'A'),(107,23,2,'A'),(107,22,2,'A'),(107,21,2,'A'),(107,20,2,'A'),(107,19,2,'A'),(107,18,2,'A'),(107,17,2,'A'),(107,16,2,'A'),(107,15,2,'A'),(107,14,2,'A'),(107,13,2,'A'),(107,12,2,'A'),(107,11,2,'A'),(107,10,2,'A'),(107,9,2,'A'),(107,8,2,'A'),(107,7,2,'A');
/*!40000 ALTER TABLE `7_6` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-04-28 12:50:51

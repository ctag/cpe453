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
-- Table structure for table `2_15`
--

DROP TABLE IF EXISTS `2_15`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `2_15` (
  `X` float default NULL,
  `Y` float default NULL,
  `TrackType` int(11) default NULL,
  `Node` text
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `2_15`
--

LOCK TABLES `2_15` WRITE;
/*!40000 ALTER TABLE `2_15` DISABLE KEYS */;
INSERT INTO `2_15` VALUES (85,13,26,'A'),(86,12,21,'A'),(86,11,2,'A'),(86,10,2,'A'),(86,9,24,'A'),(87,8,26,'A'),(88,7,26,'A'),(89,6,26,'A'),(90,5,26,'A'),(91,4,26,'A'),(92,3,17,'A'),(93,3,1,'A'),(94,3,1,'A'),(95,3,1,'A'),(96,3,1,'A'),(97,3,1,'A'),(98,3,18,'A'),(99,4,25,'A'),(100,5,25,'A'),(101,6,25,'A'),(102,7,25,'A'),(103,8,23,'A'),(103,9,2,'A'),(103,10,21,'A'),(102,11,26,'A'),(101,12,26,'A');
/*!40000 ALTER TABLE `2_15` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-04-28 12:50:54

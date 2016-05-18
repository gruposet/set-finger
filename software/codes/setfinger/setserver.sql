-- phpMyAdmin SQL Dump
-- version 4.2.12deb2+deb8u1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: 01-Abr-2016 às 09:05
-- Versão do servidor: 5.5.47-0+deb8u1
-- PHP Version: 5.6.17-0+deb8u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `setserver`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `history`
--

CREATE TABLE IF NOT EXISTS `history` (
`id` int(255) NOT NULL,
  `finger_id` int(64) NOT NULL,
  `name` varchar(255) NOT NULL,
  `timestamp` varchar(288) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `history`
--

INSERT INTO `history` (`id`, `finger_id`, `name`, `timestamp`) VALUES
(1, 1, 'Jefferson Ferreira', '1459341706'),
(2, 1, 'Jefferson Ferreira', '1459341778'),
(3, 1, 'Jefferson Ferreira', '1459341857'),
(4, 1, 'Jefferson Ferreira', '1459342273'),
(5, 1, 'Jefferson Ferreira', '1459342282'),
(6, 1, 'Jefferson Ferreira', '1459342485'),
(7, 1, 'Jefferson Ferreira', '1459342497'),
(8, 1, 'Jefferson Ferreira', '1459342510'),
(9, 1, 'Jefferson Ferreira', '1459343498'),
(10, 2, 'Caio', '1459343744'),
(11, 1, 'Jefferson Ferreira', '1459344094'),
(12, 1, 'Jefferson Ferreira', '1459344106'),
(13, 1, 'Jefferson Ferreira', '1459349810'),
(14, 1, 'Jefferson Ferreira', '1459349832'),
(15, 1, 'Jefferson Ferreira', '1459349839'),
(16, 1, 'Jefferson Ferreira', '1459352672'),
(17, 1, 'Jefferson Ferreira', '1459353511'),
(18, 1, 'Jefferson Ferreira', '1459353752'),
(19, 5, 'Fiterlinge Sousa', '1459353896'),
(20, 4, 'Gabriel Silva', '1459353907'),
(21, 1, 'Jefferson Ferreira', '1459359347'),
(22, 1, 'Jefferson Ferreira', '1459359412'),
(23, 1, 'Jefferson Ferreira', '1459361551'),
(24, 1, 'Jefferson Ferreira', '1459361576'),
(25, 1, 'Jefferson Ferreira', '1459361676'),
(26, 1, 'Jefferson Ferreira', '1459361692'),
(27, 7, 'NOVO USUARIO', '1459361740'),
(28, 7, 'Luana Oliveira', '1459361769'),
(29, 1, 'Jefferson Ferreira', '1459426101'),
(30, 6, 'Lucas Vasconcelos', '1459426256'),
(31, 6, 'Lucas Vasconcelos', '1459426283'),
(32, 6, 'Lucas Vasconcelos', '1459426350'),
(33, 1, 'Jefferson Ferreira', '1459439315'),
(34, 9, 'Paulo Natan', '1459439545'),
(35, 9, 'Paulo Natan', '1459439573'),
(36, 1, 'Jefferson Ferreira', '1459512192');

-- --------------------------------------------------------

--
-- Estrutura da tabela `ids`
--

CREATE TABLE IF NOT EXISTS `ids` (
`id` int(255) NOT NULL,
  `finger_id` int(255) NOT NULL,
  `available` int(255) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=163 DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `ids`
--

INSERT INTO `ids` (`id`, `finger_id`, `available`) VALUES
(1, 1, 0),
(2, 2, 0),
(3, 3, 0),
(4, 4, 0),
(5, 5, 0),
(6, 6, 0),
(7, 7, 0),
(8, 8, 0),
(9, 9, 0),
(10, 10, 1),
(11, 11, 1),
(12, 12, 1),
(13, 13, 1),
(14, 14, 1),
(15, 15, 1),
(16, 16, 1),
(17, 17, 1),
(18, 18, 1),
(19, 19, 1),
(20, 20, 1),
(21, 21, 1),
(22, 22, 1),
(23, 23, 1),
(24, 24, 1),
(25, 25, 1),
(26, 26, 1),
(27, 27, 1),
(28, 28, 1),
(29, 29, 1),
(30, 30, 1),
(31, 31, 1),
(32, 32, 1),
(33, 33, 1),
(34, 34, 1),
(35, 35, 1),
(36, 36, 1),
(37, 37, 1),
(38, 38, 1),
(39, 39, 1),
(40, 40, 1),
(41, 41, 1),
(42, 42, 1),
(43, 43, 1),
(44, 44, 1),
(45, 45, 1),
(46, 46, 1),
(47, 47, 1),
(48, 48, 1),
(49, 49, 1),
(50, 50, 1),
(51, 51, 1),
(52, 52, 1),
(53, 53, 1),
(54, 54, 1),
(55, 55, 1),
(56, 56, 1),
(57, 57, 1),
(58, 58, 1),
(59, 59, 1),
(60, 60, 1),
(61, 61, 1),
(62, 62, 1),
(63, 63, 1),
(64, 64, 1),
(65, 65, 1),
(66, 66, 1),
(67, 67, 1),
(68, 68, 1),
(69, 69, 1),
(70, 70, 1),
(71, 71, 1),
(72, 72, 1),
(73, 73, 1),
(74, 74, 1),
(75, 75, 1),
(76, 76, 1),
(77, 77, 1),
(78, 78, 1),
(79, 79, 1),
(80, 80, 1),
(81, 81, 1),
(82, 82, 1),
(83, 83, 1),
(84, 84, 1),
(85, 85, 1),
(86, 86, 1),
(87, 87, 1),
(88, 88, 1),
(89, 89, 1),
(90, 90, 1),
(91, 91, 1),
(92, 92, 1),
(93, 93, 1),
(94, 94, 1),
(95, 95, 1),
(96, 96, 1),
(97, 97, 1),
(98, 98, 1),
(99, 99, 1),
(100, 100, 1),
(101, 101, 1),
(102, 102, 1),
(103, 103, 1),
(104, 104, 1),
(105, 105, 1),
(106, 106, 1),
(107, 107, 1),
(108, 108, 1),
(109, 109, 1),
(110, 110, 1),
(111, 111, 1),
(112, 112, 1),
(113, 113, 1),
(114, 114, 1),
(115, 115, 1),
(116, 116, 1),
(117, 117, 1),
(118, 118, 1),
(119, 119, 1),
(120, 120, 1),
(121, 121, 1),
(122, 122, 1),
(123, 123, 1),
(124, 124, 1),
(125, 125, 1),
(126, 126, 1),
(127, 127, 1),
(128, 128, 1),
(129, 129, 1),
(130, 130, 1),
(131, 131, 1),
(132, 132, 1),
(133, 133, 1),
(134, 134, 1),
(135, 135, 1),
(136, 136, 1),
(137, 137, 1),
(138, 138, 1),
(139, 139, 1),
(140, 140, 1),
(141, 141, 1),
(142, 142, 1),
(143, 143, 1),
(144, 144, 1),
(145, 145, 1),
(146, 146, 1),
(147, 147, 1),
(148, 148, 1),
(149, 149, 1),
(150, 150, 1),
(151, 151, 1),
(152, 152, 1),
(153, 153, 1),
(154, 154, 1),
(155, 155, 1),
(156, 156, 1),
(157, 157, 1),
(158, 158, 1),
(159, 159, 1),
(160, 160, 1),
(161, 161, 1),
(162, 162, 1);

-- --------------------------------------------------------

--
-- Estrutura da tabela `users`
--

CREATE TABLE IF NOT EXISTS `users` (
`id` int(255) NOT NULL,
  `finger_id` int(64) NOT NULL,
  `user_id` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `user_name` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  `access_level` tinyint(1) NOT NULL,
  `active` tinyint(1) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `users`
--

INSERT INTO `users` (`id`, `finger_id`, `user_id`, `name`, `user_name`, `email`, `password`, `access_level`, `active`) VALUES
(1, 1, NULL, 'Jefferson Ferreira', 'jeffersonfp', 'jefferson.e.etc@gmail.com', '31fb1a2badb6162a256bdf93b1ec6e8b', 1, 0),
(2, 2, NULL, 'Caio', 'caiosbentes', 'caiosbentes@gmail.com', 'a4d55d9f92159522aee4e0bcb8fe1320', 1, 0),
(3, 3, NULL, 'Sandra Rocha', 'sandra', 'sfrtrindade@gmail.com', '00b1d0ae28a9bf82df6815c618fbbcf1', 1, 0),
(4, 4, NULL, 'Gabriel Silva', 'gabriel', NULL, 'cfab9ec82af84e8e7ff741c1b42ff6c0', 1, 0),
(5, 5, NULL, 'Fiterlinge Sousa', 'fiter', '', '64c6f113a2c61719f6e01535c3fd6c95', 1, 0),
(6, 6, NULL, 'Lucas Vasconcelos', 'lucas', NULL, '324cfebbe87c3b27eaa0afa9b497ee5d', 1, 0),
(7, 7, NULL, 'Luana Oliveira', 'luana', NULL, '98ba5150709f5d78212cd07d234cfb98', 1, 0),
(8, 8, NULL, 'Adalbery Castro', 'adalbery', 'adalbery@gmail.com', '202cb962ac59075b964b07152d234b70', 1, 0),
(10, 9, NULL, 'Paulo Natan', 'paulonatan', NULL, '827ccb0eea8a706c4c34a16891f84e7b', 1, 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `history`
--
ALTER TABLE `history`
 ADD PRIMARY KEY (`id`);

--
-- Indexes for table `ids`
--
ALTER TABLE `ids`
 ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
 ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `history`
--
ALTER TABLE `history`
MODIFY `id` int(255) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=37;
--
-- AUTO_INCREMENT for table `ids`
--
ALTER TABLE `ids`
MODIFY `id` int(255) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=163;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
MODIFY `id` int(255) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=11;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

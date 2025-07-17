USE [master]
GO
/****** Object:  Database [basar_ut]    Script Date: 02/03/2014 14:36:41 ******/
CREATE DATABASE [basar_ut] ON  PRIMARY 
( NAME = N'basar_ut', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL.1\MSSQL\DATA\basar_ut.mdf' , SIZE = 3072KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'basar_ut_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL.1\MSSQL\DATA\basar_ut_log.ldf' , SIZE = 5120KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO
EXEC dbo.sp_dbcmptlevel @dbname=N'basar_ut', @new_cmptlevel=90
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [basar_ut].[dbo].[sp_fulltext_database] @action = 'disable'
end
GO
ALTER DATABASE [basar_ut] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [basar_ut] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [basar_ut] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [basar_ut] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [basar_ut] SET ARITHABORT OFF 
GO
ALTER DATABASE [basar_ut] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [basar_ut] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [basar_ut] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [basar_ut] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [basar_ut] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [basar_ut] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [basar_ut] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [basar_ut] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [basar_ut] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [basar_ut] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [basar_ut] SET  ENABLE_BROKER 
GO
ALTER DATABASE [basar_ut] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [basar_ut] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [basar_ut] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [basar_ut] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [basar_ut] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [basar_ut] SET  READ_WRITE 
GO
ALTER DATABASE [basar_ut] SET RECOVERY FULL 
GO
ALTER DATABASE [basar_ut] SET  MULTI_USER 
GO
ALTER DATABASE [basar_ut] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [basar_ut] SET DB_CHAINING OFF 
-----------------------------------------------------------------------
USE [basar_ut]
GO
/****** Object:  Table [dbo].[but_master]    Script Date: 02/03/2014 14:36:07 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[but_master](
	[colser] [int] IDENTITY(1,1) NOT NULL,
	[colint] [int] NOT NULL CONSTRAINT [DF_but_master_colint]  DEFAULT ((0)),
	[colsmint] [smallint] NOT NULL CONSTRAINT [DF_but_master_colsmint]  DEFAULT ((0)),
	[colfl] [float] NOT NULL CONSTRAINT [DF_but_master_colfl]  DEFAULT ((0.0000000000000000)),
	[colsmfl] [float] NOT NULL CONSTRAINT [DF_but_master_colsmfl]  DEFAULT ((0.00000000)),
	[coldec] [decimal](9, 2) NOT NULL CONSTRAINT [DF_but_master_coldec]  DEFAULT ((0.00)),
	[colmon] [money] NOT NULL CONSTRAINT [DF_but_master_colmon]  DEFAULT ((0.00)),
	[colch1] [char](1) NOT NULL CONSTRAINT [DF_but_master_colch1]  DEFAULT (''),
	[colch2] [char](10) NOT NULL CONSTRAINT [DF_but_master_colch2]  DEFAULT (''),
	[colvch1] [varchar](1) NOT NULL CONSTRAINT [DF_but_master_colvchar1]  DEFAULT (''),
	[colvch2] [varchar](10) NOT NULL CONSTRAINT [DF_but_master_colvch2]  DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL CONSTRAINT [DF_but_master_colvch3]  DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL CONSTRAINT [DF_but_master_colvch4]  DEFAULT (''),
	[collvch] [text] NOT NULL CONSTRAINT [DF_but_master_collvch]  DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_master_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_master_coltime2]  DEFAULT ('23:59:59.997'),
 CONSTRAINT [PK_but_master] PRIMARY KEY CLUSTERED 
(
	[colsmint] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
-----------------------------------------------------------------------
USE [basar_ut]
GO
/****** Object:  Table [dbo].[but_detail]    Script Date: 02/03/2014 14:37:51 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[but_detail](
	[colser] [int] IDENTITY(1,1) NOT NULL,
	[colint] [int] NOT NULL CONSTRAINT [DF_but_detail_colint]  DEFAULT ((0)),
	[colsmint] [smallint] NOT NULL CONSTRAINT [DF_but_detail_colsmint]  DEFAULT ((0)),
	[colfl] [float] NOT NULL CONSTRAINT [DF_but_detail_colfl]  DEFAULT ((0.0000000000000000)),
	[colsmfl] [float] NOT NULL CONSTRAINT [DF_but_detail_colsmfl]  DEFAULT ((0.00000000)),
	[coldec] [decimal](9, 2) NOT NULL CONSTRAINT [DF_but_detail_coldec]  DEFAULT ((0.00)),
	[colmon] [money] NOT NULL CONSTRAINT [DF_but_detail_colmon]  DEFAULT ((0.00)),
	[colch1] [char](1) NOT NULL CONSTRAINT [DF_but_detail_colch1]  DEFAULT (''),
	[colch2] [char](10) NOT NULL CONSTRAINT [DF_but_detail_colch2]  DEFAULT (''),
	[colvch1] [varchar](1) NOT NULL CONSTRAINT [DF_but_detail_colvchar1]  DEFAULT (''),
	[colvch2] [varchar](10) NOT NULL CONSTRAINT [DF_but_detail_colvch2]  DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL CONSTRAINT [DF_but_detail_colvch3]  DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL CONSTRAINT [DF_but_detail_colvch4]  DEFAULT (''),
	[collvch] [text] NOT NULL CONSTRAINT [DF_but_detail_collvch]  DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coltime2]  DEFAULT ('23:59:59.997')
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
-----------------------------------------------------------------------
USE [basar_ut]
GO
/****** Object:  Table [dbo].[but_var]    Script Date: 02/03/2014 14:38:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[but_var](
	[colser] [int] IDENTITY(1,1) NOT NULL,
	[colint] [int] NOT NULL CONSTRAINT [DF_but_var_colint]  DEFAULT ((0)),
	[colsmint] [smallint] NOT NULL CONSTRAINT [DF_but_var_colsmint]  DEFAULT ((0)),
	[colfl] [float] NOT NULL CONSTRAINT [DF_but_var_colfl]  DEFAULT ((0.0000000000000000)),
	[colsmfl] [float] NOT NULL CONSTRAINT [DF_but_var_colsmfl]  DEFAULT ((0.00000000)),
	[coldec] [decimal](9, 2) NOT NULL CONSTRAINT [DF_but_var_coldec]  DEFAULT ((0.00)),
	[colmon] [money] NOT NULL CONSTRAINT [DF_but_var_colmon]  DEFAULT ((0.00)),
	[colch1] [char](1) NOT NULL CONSTRAINT [DF_but_var_colch1]  DEFAULT (''),
	[colch2] [char](10) NOT NULL CONSTRAINT [DF_but_var_colch2]  DEFAULT (''),
	[colvch1] [varchar](1) NOT NULL CONSTRAINT [DF_but_var_colvchar1]  DEFAULT (''),
	[colvch2] [varchar](10) NOT NULL CONSTRAINT [DF_but_var_colvch2]  DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL CONSTRAINT [DF_but_var_colvch3]  DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL CONSTRAINT [DF_but_var_colvch4]  DEFAULT (''),
	[collvch] [text] NOT NULL CONSTRAINT [DF_but_var_collvch]  DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_var_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_var_coltime2]  DEFAULT ('23:59:59.997')
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
-----------------------------------------------------------------------



--create procedure but_proc0
--as
--begin
--	declare @a integer;
--	set @a = 10;
--end
-------------------------------------------------------------------------
--create procedure but_proc1
--	@id integer
--as
--begin
--	declare @a integer;
--	set @a = 10 * id;
--end
-------------------------------------------------------------------------
--create function but_func0()
--returns integer
--as
--begin
--	return(10);
--end
-------------------------------------------------------------------------
--create function but_func1() 
--	returns table
--as
--	return (select 20 as ret0);
-------------------------------------------------------------------------
--create function but_func2(id int)
--	@id integer
--as
--	returns table
--	return (select id as ret0, 4* id as ret1);
--
-------------------------------------------------------------------------
--create function but_func3(id int) 
--	@id integer
--as
--	returns integer  as ret0,
--			integer  as ret1,
--			smallint as ret2;
--
--begin
--	define col0 like but_master.colser;
--	define col1 like but_master.colint;
--	define col2 like but_master.colsmint;
--	foreach
--		select colser, colint + id, colsmint - id
--		into col0, col1, col2 
--		from but_master
--		order by 1
--		return col0, col1, col2 with resume;
--end foreach;
--end



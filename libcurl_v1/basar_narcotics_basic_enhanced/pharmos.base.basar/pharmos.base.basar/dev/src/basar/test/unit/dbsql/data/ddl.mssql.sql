USE [basar_ut]
GO

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
SET ANSI_PADDING ON
GO

drop table [dbo].[but_master];
drop table [dbo].[but_detail];
drop table [dbo].[but_var];
drop procedure [dbo].[spCountMaster];
GO

CREATE PROCEDURE [dbo].[spCountMaster]
	@min   int = 0,
	@count int out
AS
	SELECT @count = count(*)
	FROM but_master 
	WHERE colint > @min
RETURN 0
GO

CREATE TABLE [dbo].[but_master](
	[colser] [int] IDENTITY(1,1) NOT NULL,
	[colint] [int] NOT NULL CONSTRAINT [DF_but_master_colint]  DEFAULT ((0)),
	[colsmint] [smallint] NOT NULL CONSTRAINT [DF_but_master_colsmint]  DEFAULT ((0)) PRIMARY KEY,
	[colfl] [float] NOT NULL CONSTRAINT [DF_but_master_colfl]  DEFAULT ((0.0000000000000000)),
	[colsmfl] [float] NOT NULL CONSTRAINT [DF_but_master_colsmfl]  DEFAULT ((0.00000000)),
	[coldec] [decimal](9, 2) NOT NULL CONSTRAINT [DF_but_master_coldec]  DEFAULT ((0.00)),
	[colmon] [money] NOT NULL CONSTRAINT [DF_but_master_colmon]  DEFAULT ((0.00)),
	[colch1] [char](1) NOT NULL CONSTRAINT [DF_but_master_colch1]  DEFAULT (''),
	[colch2] [char](10) NOT NULL CONSTRAINT [DF_but_master_colch2]  DEFAULT (''),
	[colvch1] [varchar](1) NOT NULL CONSTRAINT [DF_but_master_colvchar1]  DEFAULT (''),
	[colvch2] [varchar](10) NOT NULL DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL DEFAULT (''),
	[collvch] [text] NOT NULL DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_master_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_master_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_master_coltime2]  DEFAULT ('23:59:59.997')
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie1but_master] ON [dbo].[but_master] 
(
	[colint] ASC,
	[colsmint] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie2but_master] ON [dbo].[but_master] 
(
	[colvch3] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE UNIQUE NONCLUSTERED INDEX [xpkbut_master] ON [dbo].[but_master] 
(
	[colser] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

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
	[colvch2] [varchar](10) NOT NULL DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL DEFAULT (''),
	[collvch] [text] NOT NULL DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_detail_coltime2]  DEFAULT ('23:59:59.997')
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie1but_detail] ON [dbo].[but_detail] 
(
	[colint] ASC,
	[colsmint] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie2but_detail] ON [dbo].[but_detail] 
(
	[colvch3] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE UNIQUE NONCLUSTERED INDEX [xpkbut_detail] ON [dbo].[but_detail] 
(
	[colser] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

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
	[colvch2] [varchar](10) NOT NULL DEFAULT (''),
	[colvch3] [varchar](254) NOT NULL DEFAULT (''),
	[colvch4] [varchar](255) NOT NULL DEFAULT (''),
	[collvch] [text] NOT NULL DEFAULT (''),
	[coldate] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldate]  DEFAULT (((9999)-(12))-(31)),
	[coldt1] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldt1]  DEFAULT ('9999-12-31 23:59:59'),
	[coldt2] [datetime] NOT NULL CONSTRAINT [DF_but_var_coldt2]  DEFAULT ('9999-12-31 23:59:59.997'),
	[coltime1] [datetime] NOT NULL CONSTRAINT [DF_but_var_coltime1]  DEFAULT ('23:59:59'),
	[coltime2] [datetime] NOT NULL CONSTRAINT [DF_but_var_coltime2]  DEFAULT ('23:59:59.997')
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie1but_var] ON [dbo].[but_var] 
(
	[colint] ASC,
	[colsmint] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE NONCLUSTERED INDEX [xie2but_var] ON [dbo].[but_var] 
(
	[colvch3] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

CREATE UNIQUE NONCLUSTERED INDEX [xpkbut_var] ON [dbo].[but_var] 
(
	[colser] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]

GO

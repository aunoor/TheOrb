/* Drop Tables */

DROP TABLE IF EXISTS commodities CASCADE
;

DROP TABLE IF EXISTS commodity CASCADE
;

DROP TABLE IF EXISTS markets CASCADE
;

DROP TABLE IF EXISTS stations CASCADE
;

DROP TABLE IF EXISTS systems CASCADE
;

/* Create Tables */

CREATE TABLE commodities
(
	id bigserial NOT NULL,
	name varchar(500) NULL
)
;

CREATE TABLE commodity
(
	"marketId" bigint NOT NULL,
	"commodityId" integer NOT NULL,
	"meanPrice" integer NULL,
	"buyPrice" integer NULL,
	stock integer NULL,
	"stockBracket" integer NULL,
	"sellPrice" integer NULL,
	demand integer NULL,
	"demandBracket" integer NULL
)
;

CREATE TABLE markets
(
	"marketId" bigint NOT NULL,
	updated timestamp without time zone NOT NULL
)
;

CREATE TABLE stations
(
	"systemId64" bigint NOT NULL,
	"stationName" varchar(500) NOT NULL,
	"marketId" bigint NULL,
	"statationType" integer NOT NULL,
	"distanceToArrival" integer NULL,
	"haveMarket" boolean NULL
)
;

CREATE TABLE systems
(
	id integer NOT NULL,
	id64 bigint NOT NULL,
	"systemName" varchar(500) NOT NULL,
	x double precision NULL,
	y double precision NULL,
	z double precision NULL,
	"requirePermit" boolean NULL
)
;

/* Create Primary Keys, Indexes, Uniques, Checks */

ALTER TABLE commodities ADD CONSTRAINT "PK_commodities_id"
	PRIMARY KEY (id)
;

ALTER TABLE commodity ADD CONSTRAINT "PK_commodity_id"
	PRIMARY KEY ("marketId","commodityId")
;

CREATE INDEX "IXFK_commodity_commodities" ON commodity ("commodityId" ASC)
;

CREATE INDEX "IXFK_commodity_markets" ON commodity ("marketId" ASC)
;

ALTER TABLE markets ADD CONSTRAINT "PK_markets"
	PRIMARY KEY ("marketId")
;

CREATE INDEX "IXFK_markets_stations" ON markets ("marketId" ASC)
;

ALTER TABLE stations ADD CONSTRAINT "PK_stations_id"
	PRIMARY KEY ("systemId64")
;

CREATE INDEX "IXFK_stations_systems" ON stations ("systemId64" ASC)
;

CREATE UNIQUE INDEX "I_stations_marketId" ON stations ("marketId" ASC)
;

ALTER TABLE systems ADD CONSTRAINT "PK_systems_id64"
	PRIMARY KEY (id64)
;

CREATE INDEX "I_systemName" ON systems ("systemName" ASC)
;

/* Create Foreign Key Constraints */

ALTER TABLE commodity ADD CONSTRAINT "FK_commodity_commodities"
	FOREIGN KEY ("commodityId") REFERENCES commodities (id) ON DELETE Cascade
;

ALTER TABLE commodity ADD CONSTRAINT "FK_commodity_markets"
	FOREIGN KEY ("marketId") REFERENCES markets ("marketId") ON DELETE No Action ON UPDATE No Action
;

ALTER TABLE markets ADD CONSTRAINT "FK_markets_stations"
	FOREIGN KEY ("marketId") REFERENCES stations ("marketId") ON DELETE Cascade
;

ALTER TABLE stations ADD CONSTRAINT "FK_stations_systems"
	FOREIGN KEY ("systemId64") REFERENCES systems (id64) ON DELETE Cascade
;

/* Create Table Comments, Sequences for Autonumber Columns */



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

CREATE TABLE commodity
(
	market_id bigint NOT NULL,
	commodity_id varchar(500) NOT NULL,
	mean_price integer NULL,
	buy_price integer NULL,
	stock integer NULL,
	stock_bracket integer NULL,
	sell_price integer NULL,
	demand integer NULL,
	demand_bracket integer NULL
)
;

CREATE TABLE markets
(
	market_id bigint NOT NULL,
	updated timestamp without time zone NOT NULL
)
;

CREATE TABLE stations
(
	system_id64 bigint NOT NULL,
	station_id bigint NOT NULL,
	station_name varchar(500) NOT NULL,
	market_id bigint NULL,
	station_type varchar(500) NOT NULL,
	distance2arrival float8 NULL,
	have_market boolean NULL
)
;

CREATE TABLE systems
(
	id integer NOT NULL,
	id64 bigint NOT NULL,
	system_name varchar(500) NOT NULL,
	has_coords boolean NULL,
	x double precision NULL,
	y double precision NULL,
	z double precision NULL,
	require_permit boolean NULL
)
;

/* Create Primary Keys, Indexes, Uniques, Checks */
CREATE INDEX "IXFK_commodity_commodities" ON commodity ("commodity_id" ASC)
;

CREATE INDEX "IXFK_commodity_markets" ON commodity ("market_id" ASC)
;

ALTER TABLE markets ADD CONSTRAINT "PK_markets"
	PRIMARY KEY ("market_id")
;

CREATE INDEX "IXFK_markets_stations" ON markets ("market_id" ASC)
;

ALTER TABLE stations ADD CONSTRAINT "PK_stations_id"
	PRIMARY KEY ("station_id")
;

CREATE INDEX "IXFK_stations_systems" ON stations ("system_id64" ASC)
;

CREATE UNIQUE INDEX "I_stations_marketId" ON stations ("market_id" ASC)
;

ALTER TABLE systems ADD CONSTRAINT "PK_systems_id64"
	PRIMARY KEY (id64)
;

CREATE INDEX "I_system_name" ON systems ("system_name" ASC)
;

/* Create Foreign Key Constraints */

ALTER TABLE commodity ADD CONSTRAINT "FK_commodity_markets"
	FOREIGN KEY ("market_id") REFERENCES markets ("market_id") ON DELETE No Action ON UPDATE No Action
;

ALTER TABLE markets ADD CONSTRAINT "FK_markets_stations"
	FOREIGN KEY ("market_id") REFERENCES stations ("market_id") ON DELETE Cascade
;

ALTER TABLE stations ADD CONSTRAINT "FK_stations_systems"
	FOREIGN KEY ("system_id64") REFERENCES systems (id64) ON DELETE Cascade
;



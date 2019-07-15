/*
** Zabbix
** Copyright (C) 2001-2019 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "zbxmocktest.h"
#include "zbxmockdata.h"
#include "zbxmockutil.h"

#include "common.h"
#include "zbxalgo.h"
#include "zbxregexp.h"
#include "db.h"

void	zbx_mock_test_entry(void **state)
{
#if defined(HAVE_SQLITE3)
#	define RESULT	"out.sqlite_regex"
#else
#	define RESULT	"out.sql_regex"
#endif
	const char		*sql_where, *sql_rgx, *field_name;
	zbx_vector_uint64_t	in_ids;
	int			i, count;
	uint64_t		value_id;
	char			*sql;
	size_t			sql_alloc = 64 * ZBX_KIBIBYTE, sql_offset = 0;

	ZBX_UNUSED(state);

	sql_where = zbx_mock_get_parameter_string("in.sql_where");
	field_name = zbx_mock_get_parameter_string("in.field_name");
	value_id = zbx_mock_get_parameter_uint64("in.start_id");
	count = atoi(zbx_mock_get_parameter_string("in.count"));
	sql_rgx = zbx_mock_get_parameter_string(RESULT);
	sql = (char *)zbx_malloc(NULL, sql_alloc);
	zbx_vector_uint64_create(&in_ids);

	for (i = 0; i < count; i++)
		zbx_vector_uint64_append(&in_ids, value_id++);

	zbx_strcpy_alloc(&sql, &sql_alloc, &sql_offset, sql_where);
	DBadd_condition_alloc(&sql, &sql_alloc, &sql_offset, field_name, in_ids.values, in_ids.values_num);

	if (NULL == zbx_regexp_match(sql, sql_rgx, NULL))
	{
		printf("Prepared sql: \"%s\"\n", sql);
		fail_msg("Regular expression \"%s\" does not much sql", sql_rgx);
	}

	zbx_vector_uint64_destroy(&in_ids);
	zbx_free(sql);
}

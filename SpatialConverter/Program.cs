using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using Microsoft.SqlServer.Types;
using System.IO;

namespace SpatialConverter
{
	class Program
	{
		//const string headerLine = "segment {0}  rank 1 part {1}\n";
		//const string coordLine = "{0} {1}\n";

		private static string headerLine = "segment {0}  rank 1 part {1}";
		private static string coordLine = "{0} {1}";

		const bool UNIX_FORMAT = true;

		static void Main(string[] args)
		{
			Encoding encoding;
			if (UNIX_FORMAT)
			{
				headerLine += "\n";
				coordLine += "\n";
				encoding = Encoding.UTF8;
			}
			else
			{
				headerLine += Environment.NewLine;
				coordLine += Environment.NewLine;
				encoding = Encoding.Default;
			}

			//using (StreamWriter writer = new StreamWriter(@"C:\openFrameworks\apps\myApps\RealDirections\bin\data\unix-boundaries-simple.txt", false, encoding))
			using (StreamWriter writer = new StreamWriter(@"C:\unix-boundaries-simple.txt", false, encoding))
			{
				
				StringBuilder sb = new StringBuilder();
				using (SqlConnection con = new SqlConnection(@"Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=CountryBoundary;Data Source=10_200_11_15"))
				{
					con.Open();
					SqlCommand com = new SqlCommand("SELECT ID, geom FROM [TM_WORLD_BORDERS_SIML03] --where FIPS='SP'", con);
					//SqlCommand com = new SqlCommand("SELECT ID, geom FROM [TM_WORLD_BORDERS03] --where FIPS='SP'", con);
					//SqlCommand com = new SqlCommand("SELECT ID, geom.Reduce(0.2) as geom FROM [TM_WORLD_BORDERS_SIML03] --where FIPS='SP'", con);
					
					using (SqlDataReader reader = com.ExecuteReader())
					{
						while (reader.Read())
						{
							int numPart = 1;

							SqlGeometry geom = (SqlGeometry)reader["geom"];
							if (!geom.STIsValid().Value)
								geom = geom.MakeValid();
							if (!geom.STIsValid().Value)
								continue;

							for (int i = 1; i <= geom.STNumGeometries(); i++)
							{
								SqlGeometry curGeom = geom.STGeometryN(i);
								if (curGeom.STGeometryType().Value == OpenGisGeometryType.Polygon.ToString())
								{
									if (curGeom.STNumInteriorRing().Value > 0)
									{
										for (int intRing = 1; intRing <= curGeom.STNumInteriorRing(); intRing++)
										{
											SqlGeometry intRingGeom = curGeom.STInteriorRingN(intRing);
											WriteGeom(intRingGeom, sb, reader["ID"].ToString(), ref numPart);
										}
									}


									WriteGeom(curGeom.STExteriorRing(), sb, reader["ID"].ToString(), ref numPart);

								}
								else
								{
									WriteGeom(curGeom, sb, reader["ID"].ToString(), ref numPart);

								}
							}
						}
					}
				}


				writer.Write(sb.ToString());
			}
		}

		private static void WriteGeom(SqlGeometry geom, StringBuilder sb, string ID, ref int partNum)
		{
			sb.AppendFormat(headerLine, ID, partNum++);
			for (int j = 1; j <= geom.STNumPoints(); j++)
				sb.AppendFormat(coordLine, geom.STPointN(j).STY.ToString().Replace(",", "."), geom.STPointN(j).STX.ToString().Replace(",", "."));

		}
	}
}

extern crate noria;
use noria::SyncControllerHandle;
use std::collections::BTreeMap;
use std::thread;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};

fn main() {
    example();
}

pub(crate) fn example() {
    let mut db = ControllerHandle::from_zk(zookeeper_addr).unwrap();

    // if this is the first time we interact with Noria, we must give it the schema
    db.install_recipe("
    CREATE TABLE Article (aid int, title varchar(255), url text, PRIMARY KEY(aid));
    CREATE TABLE Vote (aid int, uid int);");

    // we can then get handles that let us insert into the new tables
    let mut article = db.table("Article").unwrap();
    let mut vote = db.table("Vote").unwrap();

    // let's make a new article
    let aid = 42;
    let title = "I love Soup";
    let url = "https://pdos.csail.mit.edu";
    article
        .insert(vec![aid.into(), title.into(), url.into()])
        .unwrap();

    // and then vote for it
    vote.insert(vec![aid.into(), 1.into()]).unwrap();

    // we can also declare views that we want want to query
    db.extend_recipe("
    VoteCount: \
      SELECT Vote.aid, COUNT(uid) AS votes \
      FROM Vote GROUP BY Vote.aid;
    QUERY ArticleWithVoteCount: \
      SELECT Article.aid, title, url, VoteCount.votes AS votes \
      FROM Article LEFT JOIN VoteCount ON (Article.aid = VoteCount.aid) \
      WHERE Article.aid = ?;");

    // and then get handles that let us execute those queries to fetch their results
    let mut awvc = db.view("ArticleWithVoteCount").unwrap();
    // looking up article 42 should yield the article we inserted with a vote count of 1
    assert_eq!(
        awvc.lookup(&[aid.into()], true).unwrap(),
        vec![vec![DataType::from(aid), title.into(), url.into(), 1.into()]]
    );
}